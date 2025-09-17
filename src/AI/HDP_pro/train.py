import os
import torch
import torch.nn as nn
from tqdm import tqdm
import yaml
import logging
from datetime import datetime

from model import HDP
from metrics import Exploitability

# 读取配置
with open('config.yaml', 'r') as f:
    config = yaml.safe_load(f)

def setup_logger(log_dir="logs"):
    """设置日志记录器"""
    # 创建日志目录
    os.makedirs(log_dir, exist_ok=True)
    
    # 生成日志文件名（包含时间戳）
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    log_filename = os.path.join(log_dir, f"training_{timestamp}.log")
    
    # 配置日志记录器
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s - %(message)s',
        handlers=[
            logging.FileHandler(log_filename, mode='w', encoding='utf-8'),
            logging.StreamHandler()  # 同时输出到控制台
        ]
    )
    
    logger = logging.getLogger(__name__)
    logger.info(f"Log file created: {log_filename}")
    return logger


class EarlyStopping:
    """Early stopping 类"""
    def __init__(self, patience=5, min_delta=1e-6, restore_best_weights=True):
        self.patience = patience
        self.min_delta = min_delta
        self.restore_best_weights = restore_best_weights
        self.wait = 0
        self.stopped_epoch = 0
        self.best_score = float('inf')
        self.best_weights = None
        self.should_stop = False
        
    def __call__(self, current_score, model=None):
        """
        检查是否应该早停
        Args:
            current_score: 当前监控的指标值
            model: 模型对象（用于保存最佳权重）
        Returns:
            bool: 是否应该停止训练
        """
        if current_score < self.best_score - self.min_delta:
            # 有改进
            self.best_score = current_score
            self.wait = 0
            if model is not None and self.restore_best_weights:
                self.best_weights = model.state_dict().copy()
        else:
            # 没有改进
            self.wait += 1
            
        if self.wait >= self.patience:
            self.should_stop = True
            
        return self.should_stop
    
    def restore_weights(self, model):
        """恢复最佳权重"""
        if self.best_weights is not None:
            model.load_state_dict(self.best_weights)


if __name__ == '__main__':

    # torch.autograd.set_detect_anomaly(True)

    # 设置日志记录器
    logger = setup_logger(config['log_dir'])

    # 检查可用设备，优先使用 cuda
    device = torch.device(config['device'] if torch.cuda.is_available() else "cpu")
    print(f"Using device: {device}")

    MAX_HEALTH = config['max_health']
    MAX_ENERGY = config['max_energy']
    ACTION_NUM = config['action_num']
    STATE_NUM = (MAX_HEALTH + 1) * (MAX_HEALTH + 1) * (MAX_ENERGY + 1) * (MAX_ENERGY + 1)
    iter_per_epoch = config['iter_per_epoch']
    epochs = config['epochs']
    total_iters = iter_per_epoch * epochs
    lr = config['lr']
    exploit_iter = config['exploit_iter']

    best_model_path = os.path.join(config['model_dir'], config['model_name'])
    os.makedirs(os.path.dirname(best_model_path), exist_ok=True)

    # 从data目录读取初始化数据
    initialization_path = config['initialization_dir']
    if not os.path.exists(initialization_path):
        raise FileNotFoundError(f"Initialization directory '{initialization_path}' does not exist.")
    transition_function = torch.load(os.path.join(initialization_path, 'transition_function.pth'))
    if transition_function.shape != (STATE_NUM, ACTION_NUM, ACTION_NUM):
        raise ValueError(f"Transition function shape mismatch: expected {(STATE_NUM, ACTION_NUM, ACTION_NUM)}, got {transition_function.shape}")
    init_policy = torch.load(os.path.join(initialization_path, 'init_policy.pth'))
    if init_policy.shape != (STATE_NUM, ACTION_NUM):
        raise ValueError(f"Initial policy shape mismatch: expected {(STATE_NUM, ACTION_NUM)}, got {init_policy.shape}")
    init_wr = torch.load(os.path.join(initialization_path, 'init_winning_rate.pth'))
    if init_wr.shape != (STATE_NUM,):
        raise ValueError(f"Initial winning rate shape mismatch: expected {(STATE_NUM,)}, got {init_wr.shape}")
    exhausting_mask = torch.load(os.path.join(initialization_path, 'energy_exhausting_mask.pth'))
    if exhausting_mask.shape != (MAX_ENERGY + 1, ACTION_NUM):
        raise ValueError(f"Exhausting mask shape mismatch: expected {(MAX_ENERGY + 1, ACTION_NUM)}, got {exhausting_mask.shape}")
    print("Initialization data loaded successfully.")

    # 创建模型并将其移动到目标设备
    hdp = HDP(MAX_HEALTH, MAX_ENERGY, ACTION_NUM, transition_function, init_policy, init_wr, exhausting_mask, wr_update_iter=5, wr_update_iter_adversarial=100).to(device)
    # 创建损失函数模块
    exploitability_calculator = Exploitability(MAX_HEALTH, MAX_ENERGY, ACTION_NUM, transition_function, init_wr, mask_std=0.8, iter_num=exploit_iter).to(device)

    # 优化器直接优化模型的参数 (hdp.parameters() 会自动包含 hdp.policy)
    optimizer = torch.optim.Adam(hdp.parameters(), lr=lr, weight_decay=1e-5)

    # # --- 添加 Polynomial 学习率调度器 ---
    # scheduler = torch.optim.lr_scheduler.PolynomialLR(
    #     optimizer, 
    #     total_iters=total_iters,  # 总的迭代次数
    #     power=0.9           # power 参数
    # )

    # # --- 添加 StepLR 学习率调度器 ---
    # scheduler = torch.optim.lr_scheduler.StepLR(
    #     optimizer, 
    #     step_size=500,  # 每500个epoch调整一次学习率
    #     gamma=0.8      # 每次将学习率乘以0.8
    # )

    # --- 添加 ReduceLROnPlateau 学习率调度器 ---
    scheduler_patience = config['scheduler']['patience']
    scheduler_cooldown = config['scheduler']['cooldown']
    min_lr = float(config['scheduler']['min_lr'])
    if config['scheduler']['type'] == 'ReduceLROnPlateau':
        scheduler = torch.optim.lr_scheduler.ReduceLROnPlateau(
            optimizer,
            mode='min',
            factor=config['scheduler']['factor'],
            patience=scheduler_patience,
            cooldown=scheduler_cooldown,
            min_lr=min_lr,
            verbose=True
        )
    else:
        raise ValueError(f"Unsupported scheduler type: {config['scheduler']['type']}. Supported types: ['ReduceLROnPlateau']")

    # --- 添加 Early Stopping 机制 ---
    early_stopping = EarlyStopping(
        patience=config['early_stopping']['patience'],
        min_delta=config['early_stopping']['min_delta'],
        restore_best_weights=True
    )

    # --- 模型指标初始化 ---
    best_exploitability_upper_bound = float('inf')
    best_exploitability_lower_bound = float('inf')
    best_hdp_model_dict = hdp.state_dict()

    # 记录训练配置
    logger.info("=== Training Configuration ===")
    logger.info(f"Epochs: {epochs}, Iterations per epoch: {iter_per_epoch}")
    logger.info(f"Learning Rate: {optimizer.param_groups[0]['lr']}")
    logger.info(f"Weight Decay: {optimizer.param_groups[0]['weight_decay']}")
    logger.info(f"Exploitability Calculation Iterations: {exploit_iter}")
    logger.info(f"Scheduler: ReduceLROnPlateau (factor={config['scheduler']['factor']}, cooldown={config['scheduler']['cooldown']}, patience={config['scheduler']['patience']}, min_lr={config['scheduler']['min_lr']})")
    logger.info(f"Device: {device}")
    logger.info(f"Model save path: {best_model_path}")
    logger.info("=" * 50)

    # --- 训练循环 ---
    logger.info("Starting Training with Exploitability as Optimization Target")

    # 用于追踪学习率是否已经到达最小值
    lr_at_min = False

    for epoch in range(epochs):
        # 创建tqdm进度条
        pbar = tqdm(range(iter_per_epoch), desc="Training")
        
        for iter in pbar:
            exploitability_calculator.train()  # 启用训练模式
            hdp.train()  # 确保模型处于训练模式
            optimizer.zero_grad()
            
            # 计算可利用性作为损失函数
            exploitability_upper_bound, exploitability_lower_bound, _, _, loss = exploitability_calculator(hdp.policy, iter_num=exploit_iter)

            # 反向传播和优化
            loss.backward()
            optimizer.step()
            
            # 获取当前学习率
            current_lr = optimizer.param_groups[0]['lr']
            
            # 计算其他指标（不参与梯度计算）
            with torch.no_grad():
                # 获取当前策略的概率分布
                current_policy_dist = torch.softmax(hdp.policy, dim=1)
                
                # 计算策略熵
                entropy = - torch.sum(current_policy_dist * torch.log(current_policy_dist + 1e-9), dim=1).mean()
                hdp.wr_update()
                
                # 更新进度条描述
                pbar.set_postfix({
                    'Epochs': f'{epoch+1}/{epochs}',
                    'Exploit': f'[{exploitability_lower_bound.item():.4f}, {exploitability_upper_bound.item():.4f}]',
                    'Entropy': f'{entropy.item():.3f}',
                    'LR': f'{current_lr:.1e}'
                })

                # --- 模型保存逻辑 ---
                if exploitability_upper_bound.item() < best_exploitability_upper_bound:
                    best_exploitability_upper_bound = exploitability_upper_bound.item()
                    best_exploitability_lower_bound = exploitability_lower_bound.item()
                    best_hdp_model_dict = hdp.state_dict()

            # if exploitability_upper_bound.item() > exploitability_lower_bound.item() + iter_num_incre_threshold:
            #     if iter_num < max_iter_num:
            #         if iter_num_incre_buffer < iter_num_incre_buffer_length - 1:
            #             iter_num_incre_buffer += 1
            #         else:
            #             iter_num += iter_num_increment
            #             iter_num_incre_buffer = 0
            #             print(f"\nIteration {iter}: Increased iter_num to {iter_num}.")

            past_policy = current_policy_dist.clone()
        
        pbar.close()

        # 检查学习率是否已经到达最小值
        current_lr = optimizer.param_groups[0]['lr']
        if current_lr <= min_lr:
            if not lr_at_min:
                lr_at_min = True
                logger.info(f"Epoch {epoch+1}: Learning rate reached minimum value {min_lr:.1e}")
                logger.info(f"Early stopping will activate after {scheduler_cooldown + scheduler_patience} epochs without improvement")
        
        # Early stopping 检查（只在学习率到达最小值后生效）
        should_stop = False
        if lr_at_min:
            should_stop = early_stopping(best_exploitability_upper_bound, hdp)
            if should_stop:
                logger.info(f"Epoch {epoch+1}: Early stopping triggered!")
                logger.info(f"No improvement for {scheduler_cooldown + scheduler_patience} epochs after reaching minimum learning rate")
                # 恢复最佳权重
                early_stopping.restore_weights(hdp)
                best_hdp_model_dict = hdp.state_dict()
        # 定期更新历史策略
        logger.info(f"Epoch {epoch+1}/{epochs}: "
                    f"Exploit=[{best_exploitability_lower_bound:.4f}, {best_exploitability_upper_bound:.4f}], "
                    f"Entropy={entropy.item():.4f}, "
                    f"LR={current_lr:.1e}, "
                    f"Loss={loss.item():.4f}")
        torch.save(best_hdp_model_dict, best_model_path)
        # print(f"  *** New best model saved with Exploitability: {best_exploitability:.6f} ***")

        scheduler.step(best_exploitability_upper_bound)

        if lr_at_min:
            logger.info(f"  Early stopping counter: {early_stopping.wait}/{scheduler_cooldown + scheduler_patience}")
        # 如果触发早停，跳出训练循环
        if should_stop:
            break


    # 训练完成日志
    final_epoch = epoch + 1
    if should_stop:
        logger.info("=" * 50)
        logger.info("Training stopped early!")
        logger.info(f"Stopped at epoch: {final_epoch}/{epochs}")
        logger.info(f"Reason: No improvement for {early_stop_patience} epochs after reaching minimum learning rate")
    else:
        logger.info("=" * 50)
        logger.info("Training completed normally!")
        logger.info(f"Completed epochs: {final_epoch}/{epochs}")
    
    logger.info(f"Best exploitability: [{best_exploitability_lower_bound:.6f}, {best_exploitability_upper_bound:.6f}]")
    logger.info(f"Best model saved at: {best_model_path}")
    logger.info(f"Final learning rate: {optimizer.param_groups[0]['lr']:.1e}")

    logger.info("Operations on the best model...")

    # torch.save(best_hdp_model_dict, best_model_path)
    # 重新创建模型实例
    best_hdp_model = HDP(MAX_HEALTH, MAX_ENERGY, ACTION_NUM, transition_function, 
                        init_policy, init_wr, exhausting_mask, 
                        wr_update_iter=5, wr_update_iter_adversarial=100).to(device)
    # 加载最佳状态
    if best_hdp_model_dict is not None:
        best_hdp_model.load_state_dict(best_hdp_model_dict)
    else:
        # 如果没有找到更好的模型，使用当前模型
        best_hdp_model.load_state_dict(hdp.state_dict())

    exploitability_calculator.eval()
    with torch.no_grad():
        exploitability_upper_bound, exploitability_lower_bound, _, _, _ = exploitability_calculator(best_hdp_model.policy, iter_num=100)
        logger.info(f"More accurate exploitability evaluation: [{exploitability_lower_bound.item():.6f}, {exploitability_upper_bound.item():.6f}]")

        # polish the model by eliminating dominated actions
        logger.info("Polishing the best model by eliminating dominated actions...")
        best_hdp_model.polish_policy(iter_num=100)
        exploitability_upper_bound, exploitability_lower_bound, _, _, _ = exploitability_calculator(best_hdp_model.policy, iter_num=100)
        logger.info(f"Exploitability after polishing: [{exploitability_lower_bound.item():.6f}, {exploitability_upper_bound.item():.6f}]")

        torch.save(best_hdp_model.state_dict(), best_model_path)
