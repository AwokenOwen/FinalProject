/*
 * Written by: AwokenOwen
 * Last Updated: March 29th 2026
 */

#pragma once
#include "Component.h"
#include "ResourceManager.h"

class AudioSource : public Component{
public:
    explicit AudioSource(Object* parent);

    void setSound(string path);

    void play();
private:
    int m_sourceId;
};
