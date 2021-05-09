#ifndef _COMMON_FUNCTION_H_
#define _COMMON_FUNCTION_H_

#include <iostream>
#include <string>
#include <pthread.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#undef main

using namespace std;

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600
#define SCREEN_BPP 32

//-1 means audio will play on 1st free channel
#define START_GAME_MUSIC_CHANNEL 1
#define IN_GAME_MUSIC_CHANNEL 2
#define EXPLOSION_CHANNEL 3
#define LASER_SHOOTING_CHANNEL 4
#define GOT_HIT_CHANNEL 5
#define BOSS_SKILL_CHANNEL 6

/*------------------------------ IMAGE ------------------------------*/
static const char s_background_img_file_path[] = "images\\galaxy.png";
static const char s_clip_spaceship_exp_img_file_path[] = "images\\exp-spaceship.png";
static const char s_clip_ufo_exp_img_file_path[] = "images\\exp-ufo.png";
static const char s_spaceshipe_img_file_path[] = "images\\spaceship95x115.png";
static const char main_background_file_path[] = "images\\startmenu.png";
static const char s_game_over_menu_img_file_path[] = "images\\game-over-menu.png";
static const char s_help_menu_img_file_path[] = "images\\help-menu.png";
static const char s_setting_menu_in_game_img_file_path[] = "images\\setting-menu-1.png";
static const char s_clip_audio_icon_img_file_path[] = "images\\clip-audio-icon.png";
static const char s_blue_sound_mark_img_file_path[] = "images\\blue-mark.png";
static const char s_green_sound_mark_img_file_path[] = "images\\green-mark.png";
static const char* s_ufo_img_file_path[] = { "images\\ufo-bad-60x56.png", "images\\ufo-good-130x120.png" };
static const char s_laser_img_file_path[] = "images\\laser.png";
static const char s_sphere_img_file_path[] = "images\\sphere.png";
static const char s_boss_img_file_path[] = "images\\boss350x210.png";
static const char s_boss_skill_img_file_path[] = "images\\boss_skill.png";
static const char s_warning_text_img_file_path[] = "images\\warning-text.png";
static const char s_victory_background_file_path[] = "images\\victory.png";
static const char s_leader_board_background_file_path[] = "images\\leaderboard.png";
static const char s_boss_background_file_path[] = "images\\boss-stage-background.jpg";

/*------------------------------ SOUND ------------------------------*/
static const char s_background_sound_file_path[] = "sounds\\Glorious-Morning.wav";
static const char s_start_game_background_sound_file_path[] = "sounds\\background-sound-2.wav";
static const char s_spaceship_exp_sound_file_path[] = "sounds\\spaceship-exp-sound.wav";
static const char s_laser_shooting_sound_file_path[] = "sounds\\laser-shooting-sound.wav";
static const char s_spaceship_got_hit_sound[] = "sounds\\spaceship-got-hit.wav";
static const char s_ufo_exp_sound_file_path[] = "sounds\\ufo-exp-sound.wav";
static const char s_boss_skill_sound_file_path[] = "sounds\\skill-sound.wav";
static const char s_danger_sound_file_path[] = "sounds\\danger.wav";
static const char s_boss_exp_sound_file_path[] = "sounds\\boss-exp-sound.wav";
static const char s_game_over_sfx_file_path[] = "sounds\\game-over-sfx.wav";

/*------------------------------ FONT ------------------------------*/
static const char s_alberto_font_file_path[] = "LNTH-Alberto-Regular.ttf";
static const int s_font_size_in_game = 40;
static const char s_showcard_gothic_font_file_path[] = "SHOWG.TTF";
static const int s_font_size_start_menu = 72;


namespace CommonFunction {

	SDL_Surface* loadImage(string file_path, const int& red = 0, const int& green = 0, const int& blue = 0);

	SDL_Rect applySurface(SDL_Surface* src, SDL_Surface* des, const int& x, const int& y);

	SDL_Rect applySurface(SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip, const int& x, const int& y);

	void cleanUp(SDL_Surface*& s_screen, SDL_Surface*& s_bkground);

	bool checkCollision(const SDL_Rect& object1, const SDL_Rect& object2);

	bool checkMouseFocusWithRect(const int& x, const int& y, const SDL_Rect& rect);

	Mix_Chunk* loadSound(const string& sound_file_path);

	void playSound(Mix_Chunk* sound, const int& channel = -1, const int loops = 0);

}
#endif // !_COMMON_FUNCTION_H_
