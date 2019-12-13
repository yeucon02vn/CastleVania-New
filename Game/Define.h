#pragma once

#define SAFE_DELETE(p) {if(p) { delete(p); p = NULL;} } 

#define BACKGROUND_COLOR D3DCOLOR_XRGB(0, 0, 0)
#define DIRECTINPUT_VERSION 0x0800

#define KEYBOARD_BUFFER_SIZE 1024	
#define WINDOW_CLASS_NAME	L"Castle Vania"
#define FULLSCREEN	false
#define MAIN_WINDOW_TITLE	L"Castle Vania"
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define FPS					200

#define GAME_FONT			L"Font\\prstart.ttf"
#define MAX_TIME_PER_FRAME		(DWORD)1000 / FPS


#define ID_TEX_BBOX -100	


#define	ID_TEX_LEVEL_ONE		1
#define	ID_TEX_LEVEL_TWO		2
#define	ID_TEX_LEVEL_THREE		3
#define ID_TEX 101

#define PLAY_TIME 300


#define GRAVITY_LOWER				0.001f
#define GRAVITY						0.0015f
#define WALLPIECES_GRAVITY			0.0005f
#define PANTHER_GRAVITY			0.001f
#define BUBBLES_GRAVITY		0.0005f
#define FISHMAN_GRAVITY 0.0015f

#define BAT_FLYING_SPEED_X	0.12f
#define BAT_FLYING_SPEED_Y	0.1f
#define ZOMBIE_WALKING_SPEED		0.1f
#define PANTHER_RUNNING_SPEED_X		0.3f
#define PANTHER_RUNNING_SPEED_Y		0.2f
#define FIREBALL_SPEED 0.13f
#define FISHMAN_WALKING_SPEED_X 0.1f
#define FISHMAN_JUMP_SPEED_Y 0.7f
#define SIMON_SPEED_Y_LOWER_ZONE    0.15f
#define SIMON_WALKING_SPEED			0.13f 
#define SIMON_WALKING_LOWER_SPEED	0.05f 
#define SIMON_JUMP_SPEED_Y			0.5f
#define SIMON_DIE_DEFLECT_SPEED		0.5f
#define SIMON_STAIR_SPEED			0.079f
#define SIMON_DEFLECT_SPEED_Y		0.3f
#define SIMON_DEFLECT_SPEED_X		0.13f
#define ITEM_FALLING_SPEED_Y		0.1f
#define ITEM_FALLING_SPEED_X		0.01f
#define ITEM_LIMITED_X				0.15f
#define WATER_SPLASSING_SPEED		0.1f
#define ITEM_Knife_SPEED			0.5f 
#define WEAPON_KNIFE_SPEED			0.3f



#define WEAPON_KNIFE	3

#define GROUND_WIDTH				32
#define GROUND_HEIGHT				32

#define LCANDLE_WIDTH				32
#define LCANDLE_HEIGHT				64

#define MCANDLE_WIDTH				16
#define MCANDLE_HEIGHT				32

#define SIMON_BBOX_WIDTH			40
#define SIMON_BBOX_HEIGHT			60

#define WHIP_BBOX_HEIGHT			20
#define WHIP_BBOX_WIDTH				40

#define BIG_HEART_BBOX_WIDTH		24
#define	BIG_HEART_BBOX_HEIGHT		20
#define SMALL_HEART_BBOX_WIDTH		16
#define	SMALL_HEART_BBOX_HEIGHT		16

#define MORNING_STAR_BBOX_WIDTH		32
#define MORNING_STAR_BBOX_HEIGHT	32

#define WEAPON_KNIFE_BBOX_WIDTH	        32
#define WEAPON_KNIFE_BBOX_HEIGHT		18

#define CHANGSCENE_BBOX		32

#define DOOR_BBOX_WIDTH 32
#define DOOR_BBOX_HEIGHT 96

#define ENEMY_DEFAULT_BBOX 32



#define SIMON_STAND_IDLE	0
#define SIMON_WALK			1
#define SIMON_SIT			2
#define SIMON_JUMP			3
#define SIMON_SIT_ATTACK	4
#define SIMON_STAND_ATTACK	5
#define SIMON_EFFECT		6
#define SIMON_STAIR_UP		7
#define SIMON_STAIR_DOWN		8
#define SIMON_HIT_STAIR_UP		9
#define SIMON_HIT_STAIR_DOWN		10
#define SIMON_DEFLECT		11
#define SIMON_DIE			12
#define SIMON_STAND_STAIR_UP		13
#define SIMON_STAND_STAIR_DOWN			14

#define SIMON_ANI_IDLE_UP_STAIR_RIGHT	19
#define SIMON_ANI_IDLE_DOWN_STAIR_RIGHT	20
#define SIMON_ANI_IDLE_UP_STAIR_LEFT	21
#define SIMON_ANI_IDLE_DOWN_STAIR_LEFT	22

#define SIMON_ANI_ATK_UP_STAIR_RIGHT	23
#define SIMON_ANI_ATK_DOWN_STAIR_RIGHT	24
#define SIMON_ANI_ATK_UP_STAIR_LEFT		25
#define SIMON_ANI_ATK_DOWN_STAIR_LEFT	26

#define SIMON_ANI_HURT_RIGHT			27
#define SIMON_ANI_HURT_LEFT				28	
#define SIMON_ANI_DIE_RIGHT				29
#define SIMON_ANI_DIE_LEFT				30

#define WHIP_ANI_NORMAL_RIGHT			0
#define WHIP_ANI_LV1_RIGHT				1
#define WHIP_ANI_LV2_RIGHT				2
#define WHIP_ANI_NORMAL_LEFT			3
#define WHIP_ANI_LV1_LEFT				4
#define WHIP_ANI_LV2_LEFT				5



#define ITEM_SMALL_HEART		 	0
#define ITEM_BIG_HEART		 		1
#define ITEM_WHIP					2
#define ITEM_KNIFE					3
#define ITEM_AXE					4
#define ITEM_BOOMERANG				5
#define ITEM_HOLYWATER				6
#define ITEM_STOP_WATCH				7


#define ENEMY_DELAY_RESPAWN_TIME	450
#define HURT_TIME	200
#define WAIT_TIME	1000
#define CHASE_TIME	300
#define SIMON_EFFECT_TIME			450
#define SIMON_ATTACK_TIME			350
#define SIMON_UNTOUCHABLE_TIME		2500
#define ITEM_TIME_DESTROYED         5000
#define EFFECTDEAD_TIME				600
#define BUBBLES_TIME			1000
#define WALLPIECES_TIME			2000
#define FISHMAN_HIT_ANI_TIME_DELAY 800
#define WHIP_NORMAL 0
#define WHIP_SHORT	1
#define WHIP_LONG	2

#define SCENE1	1
#define SCENE2	2
#define SCENE2_1 3
#define SCENE2_2 4
#define SCENE2_3 5
#define SCENE2_BOSS 6
#define SCENE3_1	7
#define SCENE3_2	8

#define GAMESTATE1 1
#define GAMESTATE2 2
#define GAMESTATE3 3

#define ANI_BIG_CANDLE		0
#define ANI_SMALL_CANDLE	1

#define ANI_GROUND_LV1		0
#define ANI_GROUND_LV2		1

#define EFFECTDEAD_STAR		0
#define EFFECTDEAD_FIRE		1

#define MAP_WIDTH_LV1	(int)1536
#define MAP_HEIGHT_LV1	(int)384
#define MAP_WIDTH_LV2	(int)5632
#define MAP_HEIGHT_LV2	(int)352
#define MAX_WIDTH_SCENE2 (int)2424
#define MAP_WIDTH_LV3	(int)1024
#define MAP_HEIGHT_LV3	(int)352
#define FRAME_TILE_MAP (int)32
#define MAP_CHECK_2	(int)3500
#define MAP_CHECK_3	(int)300
#define CANDLE 0
#define GROUND 1
#define STAIR 2
#define DOOR 3
#define ZOMBIE 4
#define PANTHER 5
#define BAT 6
#define FISHMAN 7
#define CHANGE_SCENE_OBJECT 8
#define WATER 9
#define BREAKWALL 10
#define BOSS 11

#define DOOR1 0
#define DOOR2_IDLE 1
#define DOOR2_OPEN 2

#define ZOMBIE_ACTIVE 0
#define ZOMBIE_IDLE 1

#define ZOMBIE_BBOX_WIDTH 10
#define ZOMBIE_BBOX_HEIGHT 60

#define STAIR_BBOX_WIDTH 32
#define STAIR_BBOX_HEIGHT 32

#define ZOMBIE_ACTIVE_BBOX_WIDTH 200
#define ZOMBIE_ACTIVE_BBOX_HEIGHT 200

#define HIDDEN 0
#define BREAK 1


#define BAT_ACTIVE 0
#define BAT_IDLE 1

#define BAT_BBOX_WIDTH 10
#define BAT_BBOX_HEIGHT 10

#define BAT_ACTIVE_BBOX_WIDTH 200
#define BAT_ACTIVE_BBOX_HEIGHT 200

#define PANTHER_ACTIVE 0
#define PANTHER_IDLE_INACTIVE 1
#define PANTHER_IDLE 2
#define PANTHER_JUMP	3


#define PANTHER_BBOX_WIDTH 10
#define PANTHER_BBOX_HEIGHT 32
#define PANTHER_ACTIVE_BBOX_WIDTH 100
#define PANTHER_ACTIVE_BBOX_HEIGHT 200

#define WATER_BBOX_WIDTH 512
#define WATER_BBOX_HEIGHT 32

#define FIREBALL 0
#define FIREBALL_BBOX_WIDTH 14
#define FIREBALL_BBOX_HEIGHT 12

#define FISHMAN_ACTIVE 0
#define FISHMAN_IDLE 1
#define FISHMAN_HIT 3
#define FISHMAN_JUMP 2

#define FISHMAN_BBOX_WIDTH	 10
#define FISHMAN_BBOX_HEIGHT	60
#define FISHMAN_ACTIVE_BBOX_WIDTH 200
#define FISHMAN_ACTIVE_BBOX_HEIGHT 200

#define ACTIVE 0
#define INACTIVE 1


#define BOSS_SLEEP 1
#define BOSS_HURTED 2
#define BOSS_ACTIVE 0

#define BOSS_BBOX_WIDTH 70
#define BOSS_BBOX_HEIGHT 46

#define BOSS_ACTIVE_BBOX_WIDTH 50
#define BOSS_ACTIVE_BBOX_HEIGHT 400

#define BLACKBROAD 4
#define SIMON_HP 0
#define NO_HP	1
#define BOSS_HP 2
