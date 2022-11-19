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
void InitSoundManager(void){
    AddSoundToArray(CP_Sound_Load("Assets/GameBGM.wav"),GAMEBGM,.8f);
    AddSoundToArray(CP_Sound_Load("Assets/MainMenu.wav"),MAINMENU,.1f);
    AddSoundToArray(CP_Sound_Load("Assets/EnemyDeathSound.wav"),ENEMYDEATH,1);
    AddSoundToArray(CP_Sound_Load("Assets/TetrominoExplode.wav"),TETROMINOEXPLODE,1);
    AddSoundToArray(CP_Sound_Load("Assets/Nuke.wav"),NUKE,1.2f);
    AddSoundToArray(CP_Sound_Load("Assets/LoseHealth.wav"),LOSE,1.f);
    AddSoundToArray(CP_Sound_Load("Assets/WinSound.wav"),WIN,1.f);
    AddSoundToArray(CP_Sound_Load("Assets/MouseClick.wav"),MOUSECLICK,1.f);
    AddSoundToArray(CP_Sound_Load("Assets/GameOver.wav"),GAMEOVER,.1f);
    AddSoundToArray(CP_Sound_Load("Assets/GainHealth.wav"), GAINHEART,1.f);
    AddSoundToArray(CP_Sound_Load("Assets/ZombieSpawn.wav"), ZOMBIESPAWN,.6f);
    AddSoundToArray(CP_Sound_Load("Assets/ZombieMove.wav"), ZOMBIEMOVE, 1.f);
    AddSoundToArray(CP_Sound_Load("Assets/WallBuild.wav"), WALLBUILD, .8f);
    AddSoundToArray(CP_Sound_Load("Assets/GraveSpawn.wav"), GRAVESPAWN, .7f);
    AddSoundToArray(CP_Sound_Load("Assets/GameWin.wav"), GAMEWIN, .7f);
    AddSoundToArray(CP_Sound_Load("Assets/Thud.wav"), THUD, .9f);
    AddSoundToArray(CP_Sound_Load("Assets/WallBreak.wav"), WALLBREAK, .8f);

}
//Adds sound to array of soundclips
void AddSoundToArray(CP_Sound sound,Clip clipName,float volumeModifier){
    SoundClip newSoundClip = {
        .sound = sound,
        .volumeModifier = volumeModifier,
        .clipName = clipName
    };
    SoundArray[SoundIndex] = newSoundClip;
    SoundIndex++;
}

//Stops the current BGM playing and plays provided clip as BGM
void PlayBGM(Clip clipName){
    CP_Sound_StopGroup(CP_SOUND_GROUP_MUSIC);
    CP_Sound_PlayAdvanced(GetSound(clipName), GetVolume(clipName), 1, TRUE, CP_SOUND_GROUP_MUSIC);
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

float GetVolume(Clip clipName){
    for(short i=0; i<SoundIndex; ++i){
        if (SoundArray[i].clipName == clipName){
            return SoundArray[i].volumeModifier;
        }
    }
    return 0;
}
//Plays sound set in the enum. Only use either group_SFX or group_MUSIC (Includes pitch variance)
void PlaySoundEx(Clip clipName,CP_SOUND_GROUP group){
    CP_Sound_PlayAdvanced(GetSound(clipName),GetVolume(clipName),pitch+(CP_Random_Gaussian()/4.f),FALSE,group);
}
//Plays sound set in the enum. Only use either group_SFX or group_MUSIC
void PlaySound(Clip clipName,CP_SOUND_GROUP group){
    CP_Sound_PlayAdvanced(GetSound(clipName),GetVolume(clipName),pitch,FALSE,group);
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


void StopAllSounds(){
    CP_Sound_StopAll();
}
//Stops all sounds and frees them. Remember to call on exit.
void KillSoundManager(){
    CP_Sound_StopAll();
    // for(short i=0; i< SoundIndex; ++i){
    //     CP_Sound_Free(&SoundArray[i].sound);
    // }
    SoundIndex = 0;
    memset(SoundArray, 0, sizeof(SoundClip) * SOUNDCOUNT);
}