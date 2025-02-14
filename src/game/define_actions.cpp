#include "define_actions.h"

#include <algorithm>
#include <cmath>

#include "action.h"

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
  DefineAction(-1, PRODUCE, {"PRODUCE", "qi", "."});
  DefineAction(1, 1, ATTACK, SINGLE, PISTOL, {"PISTOL", "dia", "gun"});
  DefineAction(1, 1, {1, 1}, ATTACK, SINGLE, PISTOL_UPPER,
               {"PISTOL_UPPER", "^dia", "dia^", "shangda"});
  DefineAction(1, 1, {7, 7}, ATTACK, SINGLE, PISTOL_LOWER,
               {"PISTOL_LOWER", "vdia", "diav", "Vdia", "diaV", "xiada"});
  DefineAction(1, 1, {3, 3}, ATTACK, SINGLE, PISTOL_LEFT,
               {"PISTOL_LEFT", "<dia", "dia<", "zuoda"});
  DefineAction(1, 1, {5, 5}, ATTACK, SINGLE, PISTOL_RIGHT,
               {"PISTOL_RIGHT", ">dia", "dia>", "youda"});
  DefineAction(
      2, 1, {3, 4, 5}, ATTACK, ALL, CHOP_HORIZONTAL,
      {"CHOP_HORIZONTAL", "-", "hengpi", "hengdun", "hengsao", "heng"});
  DefineAction(2, 1, {1, 4, 7}, ATTACK, ALL, CHOP_VERTICAL,
               {"CHOP_VERTICAL", "|", "shupi", "shudun", "shu"});
  DefineAction(2, 1, {0, 4, 8}, ATTACK, ALL, CHOP_INCLINE1,
               {"CHOP_INCLINE\\", "\\", "xiepi\\", "xiedun\\", "xie\\"});
  DefineAction(2, 1, {2, 4, 6}, ATTACK, ALL, CHOP_INCLINE2,
               {"CHOP_INCLINE/", "/", "xiepi/", "xiedun/", "xie/"});
  DefineAction(2, 1, {0, 1, 2}, ATTACK, ALL, CHOP_UPPER,
               {"CHOP_UPPER", "shangsao"});
  DefineAction(2, 1, {6, 7, 8}, ATTACK, ALL, CHOP_LOWER,
               {"CHOP_LOWER", "xiasao"});
  DefineAction(2, 1, {0, 3, 6}, ATTACK, ALL, CHOP_LEFT,
               {"CHOP_LEFT", "zuopi", "zuosao"});
  DefineAction(2, 1, {2, 5, 8}, ATTACK, ALL, CHOP_RIGHT,
               {"CHOP_RIGHT", "youpi", "yousao"});
  DefineAction(2, 1, {0, 1, 2, 4}, 1, {0, 1, 2, 4}, ATTACK, ALL, VISION,
               {"VISION", "T", "t", "huanshi"});
  DefineAction(2, 1, {4, 6, 7, 8}, 1, {4, 6, 7, 8}, ATTACK, ALL, HAMMER,
               {"HAMMER", "leichui", "chuizi", "langtou", "chui"});
  DefineAction(4, {1, 1, 1, 1, 0, 1, 1, 1, 1}, {1, 1, 1, 1, 0, 1, 1, 1, 1},
               ATTACK, ALL, HAWKEYE,
               {"HAWKEYE", "o", "O", "yan", "yingyan", "ying"});
  DefineAction(
      4, {0, 1, 0, 1, 2, 1, 0, 1, 0}, ATTACK, ALL, CHOP_CROSS,
      {"CHOP_CROSS", "+", "hengshu", "hengshusao", "hengshupi", "hengshudun"});
  DefineAction(4, {1, 0, 1, 0, 2, 0, 1, 0, 1}, ATTACK, ALL, CHOP_X,
               {"CHOP_X", "x", "X", "xpi", "xdun", "xsao"});
  DefineAction(4, 1, {0, 1, 2, 3, 4, 5}, ATTACK, ALL, ICEAGE,
               {"ICEAGE", "bing", "bingtian", "tianbeng"});
  DefineAction(4, 1, {3, 4, 5, 6, 7, 8}, ATTACK, ALL, EARTHQUAKE,
               {"EARTHQUAKE", "dilie", "liedi"});
  DefineAction(5, 50, {0, 1, 2, 3, 4, 5, 6, 7, 8}, 50,
               {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, SINGLE, EDITH,
               {"EDITH", "yidisi", "e", "E"});
  DefineAction(7, 70, {0, 1, 2, 3, 4, 5, 6, 7, 8}, 70,
               {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, ALL, BLACKHOLE,
               {"BLACKHOLE", "heidong"});
  DefineAction(10, 100, {0, 1, 2, 3, 4, 5, 6, 7, 8},
               100, {0, 1, 2, 3, 4, 5, 6, 7, 8}, ATTACK, ALL,
               DOOMSDAY, {"DOOMSDAY", "mori"});
  DefineAction(0, -1, SHIELD, {"SHIELD", "fang", "_|", "xiaofang"});
  DefineAction(1, -2, REMOTE_SHIELD, {"REMOTE_SHIELD", "~", "lang", "langzi"});
  DefineAction(1, -2, REBOUNDER, {"REBOUNDER", "tan", "xiaotan"});
  DefineAction(
      UpLeft, DODGE_UPLEFT,
      {"DODGE_UPLEFT", "^<", "<^", "zuoshang", "shangzuo", "zuoshangduo",
       "shangzuoduo", "zuoshangshan", "shangzuoshan", "upleft", "leftup"});
  DefineAction(UpMid, DODGE_UPMID,
               {"DODGE_UPMID", "^", "shang", "shangduo", "shangshan", "up"});
  DefineAction(
      UpRight, DODGE_UPRIGHT,
      {"DODGE_UPRIGHT", "^>", ">^", "youshang", "shangyou", "youshangduo",
       "shangyouduo", "youshangshan", "shangyoushan", "upright", "rightup"});
  DefineAction(GroundLeft, DODGE_GROUNDLEFT,
               {"DODGE_GROUNDLEFT", "<", "zuo", "zuoduo", "zuoshan", "left"});
  DefineAction(GroundRight, DODGE_GROUNDRIGHT,
               {"DODGE_GROUNDRIGHT", ">", "you", "youduo", "youshan", "right"});
  DefineAction(DownLeft, DODGE_DOWNLEFT,
               {"DODGE_DOWNLEFT", "v<", "<v", "V<", "<V", "zuoxia", "xiazuo",
                "zuoxiaduo", "xiazuoduo", "zuoxiashan", "xiazuoshan",
                "downleft", "leftdown"});
  DefineAction(DownMid, DODGE_DOWNMID,
               {"DODGE_DOWNMID", "v", "V", "xia", "xiaduo", "xiashan", "down"});
  DefineAction(DownRight, DODGE_DOWNRIGHT,
               {"DODGE_DOWNRIGHT", "v>", ">v", "V>", ">V", "youxia", "xiayou",
                "youxiaduo", "xiayouduo", "youxiashan", "xiayoushan",
                "downright", "rightdown"});
  DefineAction(0, SUICIDE, {"SUICIDE", "Q", "q", "*", "zisha", "bong"});
  DefineAction(2, ARTIFACT_SOUL, {"ARTIFACT_SOUL", "soul", "linghun", "@"});
  /*
  DefineAction(2, ARTIFACT_TIME,
               {"ARTIFACT_TIME", "time", "shijian", "jiu", "lanihuilai"});
  DefineAction(2, ARTIFACT_SPACE, {"ARTIFACT_SPACE", "space", "kongjian"});
  DefineAction(2, ARTIFACT_MIND,
               {"ARTIFACT_MIND", "mind", "spirit", "jingshen"});
  DefineAction(4, ARTIFACT_REALITY, {"ARTIFACT_REALITY", "reality", "xianshi"});
  DefineAction(3, ARTIFACT_STRENGTH,
               {"ARTIFACT_STRENGTH", "strength", "liliang"});
  */
  DefineAction(3, DUPLICATOR,
               {"DUPLICATOR", "$", "papapa", "papapapapa", "kexing"});
  DefineAction(0, 0, NONE, {"NONE", "0"});
  DefineAction(0, TIMEOUT, {"TIMEOUT", "-1"});
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  std::vector<float> effect,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, effect, type, target_type, id,
                           NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  std::vector<float> damage,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, type, target_type, id,
                           NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  float effect,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, effect, type, target_type, id,
                           NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, type, target_type, id,
                           NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> damage_range,
                  float effect,
                  std::vector<uint32_t> effect_range,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, damage_range, effect, effect_range,
                           type, target_type, id, NickNameProcess(nicknames)));
}

void DefineAction(float energy,
                  float damage,
                  std::vector<uint32_t> range,
                  ActionType type,
                  TargetType target_type,
                  uint32_t id,
                  std::vector<std::string> nicknames) {
  actions.push_back(Action(energy, damage, range, type, target_type, id,
                           NickNameProcess(nicknames)));
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