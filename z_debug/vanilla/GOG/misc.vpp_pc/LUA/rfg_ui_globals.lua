-- Constants
INVALID_THREAD_HANDLE = -1

INVALID_VDO_HANDLE = 0					-- Invalid handle when a vint_object_find() fails

INVALID_WEAPON_UNIQUE_ID = -1
INVALID_WEAPON_INVENTORY_SLOT = 0		-- This is 0 because we're starting from 1 for Lua

INVALID_NANO_INVENTORY_SLOT = 0			-- This is 0 because we're starting from 1 for Lua

TYPE_BUTTON = 0
TYPE_TOGGLE = 1
TYPE_SLIDER = 2
TYPE_GRID	= 3
TYPE_HEADER	= 4

LIST_BUTTON_SPACE = 3
LIST_BUTTON_SPACING = 32
LIST_BUTTON_HEIGHT = 32
LIST_HIGHLIGHT_HEIGHT = 10
LIST_BUTTON_TEXT_X = 48
LIST_TOGGLE_OFFSET = 0.75
LIST_SLIDER_OFFSET = 0.25

DEG_TO_RAD = 0.01745329252		-- PI/180

UINT_MAX = 0xffffffff

--setup global colors
COLOR_HIGHLIGHT_TEXT = {R=230,G=230,B=230}
COLOR_HIGHLIGHT_BAR = {R=9,G=187,B=230}
COLOR_HIGHLIGHT_BG = {R=0,G=133,B=192}

COLOR_UNHIGHLIGHT_TEXT = {R=160,G=160,B=160}
COLOR_UNHIGHLIGHT_BG = {R=40,G=40,B=40}

COLOR_DISABLED_TEXT = {R=50,G=50,B=50}
COLOR_DISABLED_BG = {R=20,G=20,B=20}

COLOR_RED = {R=182, G=36, B=18}
COLOR_RED_MEDIUM = {R=102, G=20, B=10}
COLOR_RED_DARK = {R=48, G=9, B=5}

TWEEN_STATE_WAITING	= 0
TWEEN_STATE_RUNNING	= 1
TWEEN_STATE_PAUSED	= 2
TWEEN_STATE_DISABLED	= 3
TWEEN_STATE_FINISHED	= 4

HUD_DISPOSITION_FRIENDLY	= 0
HUD_DISPOSITION_ENEMY		= 1
HUD_DISPOSITION_NEUTRAL		= 2

HUMAN_TEAM_NONE = -1
HUMAN_TEAM_GUERILLA = 0
HUMAN_TEAM_EDF = 1
HUMAN_TEAM_CIVILIAN = 2
HUMAN_TEAM_MP_NEUTRAL = 2
HUMAN_TEAM_MARAUDER = 3
HUMAN_TEAM_MP_SPECTATOR = 3

COLOR_CONTROL = {R=0.58,G=0.05,B=0}
COLOR_MORALE = {R=0.943,G=0.78,B=0.20}

-- Sound IDS. Use these with "play_ui_sound" gameside Lua function to play any sounds.
-- If you want to add to these, you need to update the UIS_ enums in gameaudio.cpp, as well as the gameside
-- functions that map the UIS_ enum values to sound IDs and the code that loads these IDs, as well as possibly
-- the table file that the event names are loaded from (game_audio.xtbl) and the global cue IDs list.
SOUND_SLIDER_ADD = ""
SOUND_SLIDER_SUBTRACT = ""
SOUND_INPUT_CONFIRM = "INT_SELECT"
SOUND_INPUT_A = SOUND_INPUT_CONFIRM -- Confirm
SOUND_INPUT_CANCEL = "INT_BACK"
SOUND_INPUT_B = SOUND_INPUT_CANCEL -- Cancel/Back
SOUND_INPUT_MISC = ""
SOUND_INPUT_X = SOUND_INPUT_MISC
SOUND_INPUT_OTHER = ""
SOUND_INPUT_Y = SOUND_INPUT_OTHER
SOUND_SCROLL_UP = "INT_TOGGLE_UD" -- Dpad stuff
SOUND_SCROLL_DOWN = "INT_TOGGLE_UD" -- Dpad stuff
SOUND_SCROLL_LEFT = "INT_TOGGLE_UD" -- Dpad stuff
SOUND_SCROLL_RIGHT = "INT_TOGGLE_UD" -- Dpad stuff
SOUND_BUMPER_RIGHT = "INT_TRIGGER" -- Use these
SOUND_BUMPER_LEFT = "INT_TRIGGER" -- Use these
SOUND_TRIGGER_RIGHT = "INT_TRIGGER" -- Use these
SOUND_TRIGGER_LEFT = "INT_TRIGGER" -- Use these
SOUND_INPUT_INVALID = "INT_ERROR" -- Option does not exist
SOUND_LEFT_STICK_PRESS = ""
SOUND_RIGHT_STICK_PRESS = ""
SOUND_DPAD_UP = "INT_TOGGLE_UD"
SOUND_DPAD_DOWN = "INT_TOGGLE_UD"
SOUND_DPAD_RIGHT = "INT_TOGGLE_UD"
SOUND_DPAD_LEFT = "INT_TOGGLE_UD"

SOUND_AMBIENT_ADJUST = ""
SOUND_MUSIC_VOLUME_ADJUST = ""
SOUND_SFX_VOLUME_ADJUST = ""
SOUND_VOICE_VOLUME_ADJUST = ""
SOUND_OVERALL_VOLUME_ADJUST = ""

SOUND_WEAPON_RADIAL_OPEN = 26 -- Open weapons
SOUND_WEAPON_RADIAL_CLOSE = 27 -- Close weapons
SOUND_WEAPON_RADIAL_HIGHLIGHT_NEW_WEAPON = 28 -- Move the cursor on the weapon
SOUND_WEAPON_RADIAL_SELECT_WEAPON = 29 -- Pick weapon on radial
SOUND_NANOFORGE_RADIAL_OPEN = 30 -- Open nano
SOUND_NANOFORGE_RADIAL_CLOSE = 31 -- Close nano
SOUND_NANOFORGE_RADIAL_HIGHLIGHT_NEW_ABILITY = 32 -- Move cursor on nano

SOUND_WEAPON_UPGRADE_SCROLL_UP = 33
SOUND_WEAPON_UPGRADE_SCROLL_DOWN = 34
SOUND_WEAPON_UPGRADE_SELECT_UPGRADE = 35
SOUND_WEAPON_UPGRADE_HIGHLIGHT_NEW_UPGRADE = 36
SOUND_WEAPON_UPGRADE_PURCHASE_STANDARD_UPGRADE = 37
SOUND_WEAPON_UPGRADE_PURCHASE_SUPER_UPGRADE = 38
SOUND_WEAPON_UPGRADE_SCREEN_OPEN = 39
SOUND_WEAPON_UPGRADE_SCREEN_CLOSE = 40

SOUND_PAUSE_MENU_OPEN = 43
-- next unused number: 44

TEXT_CHAT_INPUT_PRIORITY = 998

-- Global Return Screen Values
Global_last_main_menu_tier = 1
Global_last_main_menu_option = 1

Global_show_kb_controls = false

function main_menu_reset_selections()
	Global_last_main_menu_tier = 1
	Global_last_main_menu_option = 1
end

function main_menu_setup_for_online_return()
	-- The magic numbers here must match the tier and option in ui_main_menu for the online sub-menu
	Global_last_main_menu_tier = 2
	Global_last_main_menu_option = 1
end

Global_last_pause_tier = 1
Global_last_pause_option = 1

function pause_menu_reset_selections()
	Global_last_pause_tier = 1
	Global_last_pause_option = 1
end

Global_last_lobby_tier = 1
Global_last_lobby_option = 1

Global_last_wrecking_crew_option = -1

function multi_player_lobby_reset()
	Global_last_lobby_tier = 1
	Global_last_lobby_option = 1
end

Global_last_dlc_menu_tier = 1
Global_last_dlc_menu_option = 1

function dlc_menu_reset_selections()
	Global_last_dlc_menu_tier = 1
	Global_last_dlc_menu_option = 1
end