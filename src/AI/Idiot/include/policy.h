#pragma once

#include <functional>

#include "../../../game/define_actions.h"
#include "tensor.h"

namespace AI {

namespace Idiot {

const uint32_t STATE_DIM = 2;
const uint32_t ACTION_NUM = 38;
const uint32_t MAX_ENERGY = 20;
const uint32_t MAX_HEALTH = 10;
const float UPDATE_PRECISION = 100.0;
const float GAMMA = 0.9;

class Reward : public Tensor<float, STATE_DIM * 2 + 1> {
    private:
        uint32_t id_;
    public:
        Reward();
        Reward(uint32_t id);
        Reward(const Reward &r) : Tensor<float, STATE_DIM * 2 + 1>(r), id_(r.id_) {}
        Reward &operator=(const Reward &r);
        Reward operator+(const Reward &r);
        Reward operator-(const Reward &r);
        Reward operator*(const float f);
        Reward &operator+=(const Reward &r);
        Reward &operator-=(const Reward &r);
        Reward &operator*=(const float f);
        ~Reward() {}

        void Update(float enemy_health, float enemy_energy, float health, float energy, Game::Action *action);
        void Store(const std::string &path);

        inline uint32_t GetId() const { return id_; }
};

class Policy : public Tensor<float, STATE_DIM * 2 + 1> {
    private:
        std::string name_;
        uint32_t id_;

    public:
        Policy();
        Policy(uint32_t id);
        Policy(const Policy &p) : Tensor<float, STATE_DIM * 2 + 1>(p), name_(p.name_), id_(p.id_) {}
        Policy(const std::string &path);
        Policy &operator=(const Policy &p);
        Policy operator+(const Reward &r);
        Policy operator*(const Reward &r);
        Policy &operator+=(const Reward &r);
        Policy &operator*=(const Reward &r);
        ~Policy() {}

        void Store(const std::string &path);
        void Store(const std::string &path, const std::string &name);
        void Normalize();
        void Update(Reward &r);
        void RewardFeedback(Reward &r);
        Game::Action *GetAction(uint32_t health, uint32_t energy, uint32_t enemy_health, uint32_t enemy_energy);

        inline std::string GetName() const { return name_; }
        inline uint32_t GetId() const { return id_; }
};

}

}