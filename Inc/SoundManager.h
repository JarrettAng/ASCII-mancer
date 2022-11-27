/*!
@file	  SoundManager.h
@author	  Amadeus Chia (amadeusjinhan.chia@digipen.edu)
@date     21/11/2022
@brief    This header file contains the information about the sound manager and the functions that can be called from it.

@license  Copyright © 2022 DigiPen, All rights reserved.
________________________________________________________________________________________________________*/

#pragma once
#include <cprocessing.h>

#define SOUNDCOUNT 25   //designating 25 sounds as the max count

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
    ZOMBIEMOVE,
    WALLBUILD,
    WALLBREAK,
    GRAVESPAWN,
    GAMEWIN,
    THUD,
    NONE
} Clip;

//Sound clips are just sounds with clipnames tagged to them
typedef struct SoundClip{
    CP_Sound sound;
    float volumeModifier;
    Clip clipName;
} SoundClip;
/*____________________________________________________________________*/


/*
@brief Loads all the sounds in the game. Needs to be called once and only once!
*/
void InitSoundManager(void);
/*____________________________________________________________________*/

/*
@brief Plays the specified BGM (LOOPS)
@param[in] clipName is the name of the clip. (Refer to Clip enum)
*/
void PlayBGM(Clip clipName);
/*____________________________________________________________________*/

/*
@brief Stops all sounds that are playing
*/
void StopAllSounds(void);
/*____________________________________________________________________*/

/*
@brief Adds the loaded sound into the array and assigns the names and base volumes to them.
@param[in] clipName is the name of the clip. (Refer to Clip enum)
@param[in] sound is the CP_Sound_Load("PathName") for the sound
@param[in] volumeModifier is the base volume of the sound (for when they are too loud)
*/
void AddSoundToArray(CP_Sound sound,Clip clipName,float volumeModifier);
//Returns a sound that matches the clip name. NULL if clipname doesn't exist.
/*____________________________________________________________________*/

/*
@brief Returns the specified sound if it exists. NULL if it doesn't
@param[in] clipName is the name of the clip. (Refer to Clip enum)
*/
CP_Sound GetSound(Clip clipName);
/*____________________________________________________________________*/

/*
@brief Returns the base volume of the specified clip. 
@param[in] clipName is the name of the clip. (Refer to Clip enum)
*/
float GetVolume(Clip clipName);
/*____________________________________________________________________*/

/*
@brief Plays the specified sound with varied pitch and sets the sound group. Use only GROUP_SFX or GROUP_MUSIC accordingly!
@param[in] clipName the name of the clip. (Refer to Clip enum)
@param[in] group the group that the sound belongs too (GROUP_SFX or GROUP_MUSIC)
*/
void PlaySoundEx(Clip clipName,CP_SOUND_GROUP group);
/*____________________________________________________________________*/

/*
@brief Plays the specified sound and sets the sound group. Use only GROUP_SFX or GROUP_MUSIC accordingly!
@param[in] clipName the name of the clip. (Refer to Clip enum)
@param[in] group the group that the sound belongs too (GROUP_SFX or GROUP_MUSIC)
*/
void PlaySound(Clip clipName,CP_SOUND_GROUP group);
/*____________________________________________________________________*/

/*
@brief Sets GROUP_SFX volume
@param[in] volume the volume to set the SFX to.
*/
void SetSFXVolume(float volume);
/*____________________________________________________________________*/

/*
@brief Sets GROUP_MUSIC volume
@param[in] volume the volume to set the BGM to.
*/
void SetBGMVolume(float volume);
/*____________________________________________________________________*/

/*
@brief Toggles mute for ALL sounds.
*/
void ToggleMuteALL(void);
/*____________________________________________________________________*/

/*
@brief Toggles mute for GROUP_SFX
*/
void ToggleMuteSFX(void);
/*____________________________________________________________________*/

/*
@brief Toggles mute for GROUP_MUSIC
*/
void ToggleMuteBGM(void);