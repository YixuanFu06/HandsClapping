#include "define_actions.h"

#include <algorithm>
#include <cmath>

#include "action.h"
#include "player.h"

namespace Game {

std::vector<Action> actions;

std::vector<std::string> AllNicknames;

std::vector<std::string> NickNameProcess(std::vector<std::string> nicknames) {
  std::string lower_nickname = nicknames[0];
  std::transform(lower_nickname.begin(), lower_nickname.end(),
                 lower_nickname.begin(), ::tolower);
  nicknames.insert(nicknames.begin() + 1, lower_nickname);
  std::vector<std::string> new_nicknames;
  for (std::string &nickname : nicknames) {
    if (std::find(AllNicknames.begin(), AllNicknames.end(), nickname) !=
        AllNicknames.end()) {
      std::cout << "Nickname " << nickname << " of action " << nicknames[0]
                << " is been used. Ignored here.\n";
      continue;
    }
    AllNicknames.push_back(nickname);
    new_nicknames.push_back(nickname);
  }
  return new_nicknames;
}

void InitActions() {
  DefineAction(0, 0, NONE, {"NONE", "0"});
  DefineAction(-1, PRODUCE, {"PRODUCE", "qi", "1", "气"});
  DefineAction(1, 1, ATTACK, PISTOL, {"PISTOL", "dia", "gun", "枪"});
  DefineAction(1, 1, {1, 1}, ATTACK, PISTOL_UPPER,
               {"PISTOL_UPPER", "^dia", "dia^", "shangda", "上打"});
  DefineAction(
      1, 1, {7, 7}, ATTACK, PISTOL_LOWER,
      {"PISTOL_LOWER", "vdia", "diav", "Vdia", "diaV", "xiada", "下打"});
  DefineAction(1, 1, {3, 3}, ATTACK, PISTOL_LEFT,
               {"PISTOL_LEFT", "<dia", "dia<", "zuoda", "左打"});
  DefineAction(1, 1, {5, 5}, ATTACK, PISTOL_RIGHT,
               {"PISTOL_RIGHT", ">dia", "dia>", "youda", "右打"});
  DefineAction(2, 1, {3, 4, 5}, ATTACK, CHOP_HORIZONTAL,
               {"CHOP_HORIZONTAL", "-", "——", "hengpi", "hengdun", "hengsao",
                "heng", "横扫"});
  DefineAction(2, 1, {1, 4, 7}, ATTACK, CHOP_VERTICAL,
               {"CHOP_VERTICAL", "|", "shupi", "shudun", "shu", "竖劈"});
  DefineAction(
      2, 1, {0, 4, 8}, ATTACK, CHOP_INCLINE1,
      {"CHOP_INCLINE1", "\\", "、", "xiepi1", "xiedun1", "xie1", "斜劈1"});
  DefineAction(2, 1, {2, 4, 6}, ATTACK, CHOP_INCLINE2,
               {"CHOP_INCLINE2", "/", "xiepi2", "xiedun2", "xie2", "斜劈2"});
  DefineAction(2, 1, {0, 1, 2}, ATTACK, CHOP_UPPER,
               {"CHOP_UPPER", "shangsao", "上扫"});
  DefineAction(2, 1, {6, 7, 8}, ATTACK, CHOP_LOWER,
               {"CHOP_LOWER", "xiasao", "下扫"});
  DefineAction(2, 1, {0, 3, 6}, ATTACK, CHOP_LEFT,
               {"CHOP_LEFT", "zuopi", "zuosao", "左劈"});
  DefineAction(2, 1, {2, 5, 8}, ATTACK, CHOP_RIGHT,
               {"CHOP_RIGHT", "youpi", "yousao", "右劈"});
  DefineAction(2, 1, {0, 1, 2, 4}, 1, {0, 1, 2, 4}, ATTACK, VISION,
               {"VISION", "T", "t", "huanshi"});
  DefineAction(2, 1, {4, 6, 7, 8}, 1, {4, 6, 7, 8}, ATTACK, HAMMER,
               {"HAMMER", "leichui", "chuizi", "langtou", "chui", "锤子", "⊥"});
  DefineAction(4, {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
               ATTACK, HAWKEYE,
               {"HAWKEYE", "o", "O", "yan", "yingyan", "ying"});
  DefineAction(4, {0, 1, 0, 1, 2, 1, 0, 1, 0}, ATTACK, CHOP_CROSS,
               {"CHOP_CROSS", "+", "hengshu", "hengshusao", "hengshupi",
                "hengshudun", "横竖扫"});
  DefineAction(4, {1, 0, 1, 0, 2, 0, 1, 0, 1}, ATTACK, CHOP_X,
               {"CHOP_X", "x", "X", "xpi", "xdun", "xsao", "双斜扫"});
  DefineAction(4, 1, {0, 1, 2, 3, 4, 5}, ATTACK, ICEAGE,
               {"ICEAGE", "bing", "bingtian", "tianbeng", "冰天"});
  DefineAction(4, 1, {3, 4, 5, 6, 7, 8}, ATTACK, EARTHQUAKE,
               {"EARTHQUAKE", "dilie", "liedi", "地裂"});
  DefineAction(5, std::pow(10, 5), {0, 1, 2, 3, 4, 5, 6, 7, 8}, std::pow(10, 5),
               {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, EDITH,
               {"EDITH", "yidisi", "e", "E"});
  DefineAction(7, std::pow(10, 7), {0, 1, 2, 3, 4, 5, 6, 7, 8}, std::pow(10, 7),
               {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, BLACKHOLE,
               {"BLACKHOLE", "heidong", "黑洞"});
  DefineAction(10, std::pow(10, 10), {0, 1, 2, 3, 4, 5, 6, 7, 8},
               std::pow(10, 10), {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, DOOMSDAY,
               {"DOOMSDAY", "mori", "末日"});
  DefineAction(0, -1, SHIELD, {"SHIELD", "fang", "_|", "xiaofang", "盾"});
  DefineAction(1, -2, REMOTE_SHIELD,
               {"REMOTE_SHIELD", "~", "lang", "langzi", "遥控盾"});
  DefineAction(1, -2, REBOUNDER, {"REBOUNDER", "tan", "xiaotan", "小弹"});
  DefineAction(UpLeft, DODGE_UPLEFT,
               {"DODGE_UPLEFT", "^<", "<^", "zuoshang", "shangzuo",
                "zuoshangduo", "shangzuoduo", "zuoshangshan", "shangzuoshan",
                "upleft", "leftup", "左上躲"});
  DefineAction(
      UpMid, DODGE_UPMID,
      {"DODGE_UPMID", "^", "shang", "shangduo", "shangshan", "up", "上躲"});
  DefineAction(UpRight, DODGE_UPRIGHT,
               {"DODGE_UPRIGHT", "^>", ">^", "youshang", "shangyou",
                "youshangduo", "shangyouduo", "youshangshan", "shangyoushan",
                "upright", "rightup", "右上躲"});
  DefineAction(
      GroundLeft, DODGE_GROUNDLEFT,
      {"DODGE_GROUNDLEFT", "<", "zuo", "zuoduo", "zuoshan", "left", "左躲"});
  DefineAction(
      GroundRight, DODGE_GROUNDRIGHT,
      {"DODGE_GROUNDRIGHT", ">", "you", "youduo", "youshan", "right", "右躲"});
  DefineAction(DownLeft, DODGE_DOWNLEFT,
               {"DODGE_DOWNLEFT", "v<", "<v", "V<", "<V", "zuoxia", "xiazuo",
                "zuoxiaduo", "xiazuoduo", "zuoxiashan", "xiazuoshan",
                "downleft", "leftdown", "左下躲"});
  DefineAction(
      DownMid, DODGE_DOWNMID,
      {"DODGE_DOWNMID", "v", "V", "xia", "xiaduo", "xiashan", "down", "下躲"});
  DefineAction(DownRight, DODGE_DOWNRIGHT,
               {"DODGE_DOWNRIGHT", "v>", ">v", "V>", ">V", "youxia", "xiayou",
                "youxiaduo", "xiayouduo", "youxiashan", "xiayoushan",
                "downright", "rightdown", "右下躲"});
  DefineAction(0, SUICIDE, {"SUICIDE", "Q", "q", "#", "zisha", "bong", "自杀"});
  DefineAction(2, ARTIFACT_SOUL,
               {"ARTIFACT_SOUL", "soul", "linghun", "@", "灵魂"});
  DefineAction(
      2, ARTIFACT_TIME,
      {"ARTIFACT_TIME", "time", "shijian", "jiu", "lanihuilai", "时间"});
  DefineAction(2, ARTIFACT_SPACE,
               {"ARTIFACT_SPACE", "space", "kongjian", "空间"});
  DefineAction(2, ARTIFACT_MIND,
               {"ARTIFACT_MIND", "mind", "spirit", "jingshen", "精神"});
  DefineAction(4, ARTIFACT_REALITY,
               {"ARTIFACT_REALITY", "reality", "xianshi", "现实"});
  DefineAction(3, ARTIFACT_STRENGTH,
               {"ARTIFACT_STRENGTH", "strength", "liliang", "力量"});
  DefineAction(3, DUPLICATOR,
               {"DUPLICATOR", "$", "￥", "papapa", "papapapapa", "kexing"});
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, effect, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, effect, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, damage_range, effect, effect_range,
                           type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(
      Action(energy, damage, range, type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float effect,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, effect, id, NickNameProcess(nicknames)));
}

void DefineAction(PlayerPosition dodge_position,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(dodge_position, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, id, NickNameProcess(nicknames)));
}

}  // namespace Game