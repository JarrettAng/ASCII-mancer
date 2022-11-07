#pragma once
#include <cprocessing.h>

#define SOUNDCOUNT 25
//Maybe do #define SFX CP_SOUND_GROUP_SFX
//Maybe do #define BGM CP_SOUND_GROUP_MUSIC


//Enum for clipnames. Add clip names here if more sounds needed
typedef enum Clip{
    GAMEBGM,
    MAINMENU,
    ENEMYDEATH,
    TETROMINOEXPLODE,
    NUKE,
    WIN,
    LOSE,
    MOUSECLICK,
    GAMEOVER,
    GAINHEART,
    ZOMBIESPAWN,
    NONE
} Clip;

//Sound clips are just sounds with clipnames tagged to them
typedef struct SoundClip{
    CP_Sound sound;
    float volumeModifier;
    Clip clipName;
}SoundClip;


//========= MUST DECLARE FUNCTIONS ========================

//Inits sounds, and also starts the BGM playing
void InitSoundManager(Clip clipName);
//Clears the sound array and frees up the sounds from memory.
void KillSoundManager(void);

//Used in Init to add sounds to the array
void AddSoundToArray(CP_Sound sound,Clip clipName,float volumeModifier);
//Returns a sound that matches the clip name. NULL if clipname doesn't exist.
CP_Sound GetSound(Clip clipName);
float GetVolume(Clip clipName);
//Plays Sound with varied pitch and sets the sound group. Use only GROUP_SFX or GROUP_MUSIC accordingly!
void PlaySoundEx(Clip clipName,CP_SOUND_GROUP group);
void PlaySound(Clip clipName,CP_SOUND_GROUP group);


//Sets GROUP_SFX volume
void SetSFXVolume(float volume);
//Sets GROUP_MUSIC volume
void SetBGMVolume(float volume);
//Toggles mute for all 
void ToggleMuteALL(void);
//Toggles mute for GROUP_SFX
void ToggleMuteSFX(void);
//Toggles mute for GROUP_MUSIC
void ToggleMuteBGM(void);