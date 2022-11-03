#include "SoundManager.h"

//Sound Array 
SoundClip SoundArray[SOUNDCOUNT];          //Assuming we have 25 sounds LMAO.
int SoundIndex = 0;

float volume =1.0f;     //original sound, no modification
float pitch = 1.0f;     

_Bool toggleMuteAll = FALSE;
_Bool toggleMuteSFX = FALSE;
_Bool toggleMuteBGM = FALSE;


//Inits sounds to soundclip array and PLAYS BGM. 
void InitSoundManager(Clip BGMName){
    AddSoundToArray(CP_Sound_Load("Assets/GameBGM.wav"),GAMEBGM);
    AddSoundToArray(CP_Sound_Load("Assets/MainMenu.wav"),MAINMENU);
    AddSoundToArray(CP_Sound_Load("Assets/EnemyDeathSound.wav"),ENEMYDEATH);
    AddSoundToArray(CP_Sound_Load("Assets/TetrominoExplode.wav"),TETROMINOEXPLODE);
    AddSoundToArray(CP_Sound_Load("Assets/Nuke.wav"),NUKE);
    AddSoundToArray(CP_Sound_Load("Assets/LoseHealth.wav"),LOSE);
    AddSoundToArray(CP_Sound_Load("Assets/WinSound.wav"),WIN);
    AddSoundToArray(CP_Sound_Load("Assets/MouseClick.wav"),MOUSECLICK);
    AddSoundToArray(CP_Sound_Load("Assets/GameOver.wav"),GAMEOVER);

    if (BGMName == NONE) return;
    CP_Sound_PlayMusic(GetSound(BGMName));
}

//Adds sound to array of soundclips
void AddSoundToArray(CP_Sound sound,Clip clipName){
    SoundClip newSoundClip = {
        .sound = sound,
        .clipName = clipName
    };
    SoundArray[SoundIndex] = newSoundClip;
    SoundIndex++;
}

//Returns sound from SoundArray based off clip name
CP_Sound GetSound(Clip clipName){
    for(short i=0; i<SoundIndex; ++i){
        if (SoundArray[i].clipName == clipName){
            return SoundArray[i].sound;
        }
    }
    return NULL;
}

//Plays sound set in the enum. Only use either group_SFX or group_MUSIC (Includes pitch variance)
void PlaySoundEx(Clip clipName,CP_SOUND_GROUP group){
    CP_Sound_PlayAdvanced(GetSound(clipName),volume,pitch+(CP_Random_Gaussian()/4.f),FALSE,group);
}
//Plays sound set in the enum. Only use either group_SFX or group_MUSIC
void PlaySound(Clip clipName,CP_SOUND_GROUP group){
    CP_Sound_PlayAdvanced(GetSound(clipName),volume,pitch,FALSE,group);
}
//Function that sets sfx volume. 1.0f default, 0.f is silence.
void SetSFXVolume(float volume){
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_SFX,volume);
}

//Function that sets BGM volume. 1.0f default, 0.f is silence.
void SetBGMVolume(float volume){
    CP_Sound_SetGroupVolume(CP_SOUND_GROUP_MUSIC,volume);
}

//Toggles mute on ALL sounds. Uses Pause and Resume for sounds.
void ToggleMuteALL(void){
    toggleMuteAll = !toggleMuteAll;
    toggleMuteAll ? CP_Sound_PauseAll() : CP_Sound_ResumeAll();
}

//Toggles SFX Mute. Uses Pause and Resume for group_SFX
void ToggleMuteSFX(void){
    toggleMuteSFX = !toggleMuteSFX;
    toggleMuteSFX ? CP_Sound_PauseGroup(CP_SOUND_GROUP_SFX) : CP_Sound_ResumeGroup(CP_SOUND_GROUP_SFX);
}
//Toggles MUSIC Mute. Uses Pause and Resume for group_MUSIC
void ToggleMuteBGM(void){
    toggleMuteSFX = !toggleMuteSFX;
    toggleMuteSFX ? CP_Sound_PauseGroup(CP_SOUND_GROUP_MUSIC) : CP_Sound_ResumeGroup(CP_SOUND_GROUP_MUSIC);
}

//Stops all sounds and frees them. Remember to call on exit.
void KillSoundManager(){
    CP_Sound_StopAll();
    for(short i=0; i< SoundIndex; ++i){
        CP_Sound_Free(SoundArray[i].sound);
    }
    SoundIndex = 0;
    memset(SoundArray, 0, sizeof(SoundClip) * SOUNDCOUNT);
}