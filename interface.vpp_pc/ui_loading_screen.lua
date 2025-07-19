local LOADING_METER_START_SCALE_X	= 0
local LOADING_METER_END_SCALE_X		= 302
local LOADING_METER_SCALE_Y			= 5

local Loading_install_warning

local Heading_text
local Loading_text
local Heading_text_glow
local Loading_text_glow

local Meter_anim
local Meter_tween
--local Loading_meter

local Install_processing = false

function ui_loading_screen_init()
	Loading_install_warning = Vdo_base_object:new("loading_install_warning")
	Loading_install_warning:set_visible(false)

	Meter_tween = Vdo_tween_object:new("loading_meter_scale_twn_2")
	--Meter_anim = Vdo_anim_object:new("loading_meter_anim")
	
	Loading_meter = Vdo_base_object:new("loading_meter")
	-- Start the meter as unfilled
	Loading_meter:set_scale(LOADING_METER_START_SCALE_X, LOADING_METER_SCALE_Y)
	
	Heading_text = Vdo_base_object:new("heading")
	Heading_text_glow = Vdo_base_object:new("heading_glow")
	Heading_text:set_text("")
	Heading_text_glow:set_text("")
	
	Loading_text = Vdo_base_object:new("loading_text")
	Loading_text_glow = Vdo_base_object:new("loading_text_glow")
	Loading_text:set_text("")
	Loading_text_glow:set_text("")
	
	-- Set up the come-in anim for the meter alpha to start the meter running
	local meter_alpha_tween = Vdo_tween_object:new("loading_meter_group_alpha_twn_1")
	meter_alpha_tween:set_end_event("ui_loading_screen_meter_done")
	
	vint_dataitem_add_subscription("loading_text_data_item", "update", "ui_loading_screen_text_update")
	
	-- Start all the animations
	--local anims = {
		--"loading_anim_in",
		--"loading_file_group_anim",
		--"ring_animation",
		--"ring_glow_animation",
		--"video_ring_anim",
	--}
	
	--lua_batch_play_anims(anims)
end

function ui_loading_screen_cleanup()
end

local function set_random_loading_meter()
	local random_duration = rand_int(0.5, 6.0)
	Meter_tween:set_property("duration", random_duration)
	Meter_tween:set_end_event("ui_loading_screen_meter_done")
	--Meter_anim:play()
end

-- Returns a bool for whether localization is ready.  If it is false, DO NOT SHOW ANY STRINGS ON SCREEN
function ui_loading_screen_text_update(di_h, event_type)
	local localization_ready, installing, install_status, install_percent = vint_dataitem_get(di_h)
	
	if localization_ready then
		local heading = "MENU_INITIALIZE"
	
		if installing then
			heading = install_status
			Loading_install_warning:set_visible(true)
			
			-- Set the text here again so it shows up localized
			Loading_install_warning:set_text("PLT_CACHE_MESSAGE")
		else
			Loading_install_warning:set_visible(false)
		end
	
		Heading_text:set_text(heading)
		Heading_text_glow:set_text(heading)
		
		local loading = "MENU_LOADING"
		Loading_text:set_text(loading)
		Loading_text_glow:set_text(loading)
	else
		Heading_text:set_text("")
		Heading_text_glow:set_text("")
		Loading_text:set_text("")
		Loading_text_glow:set_text("")
		Loading_install_warning:set_visible(false)
	end
	
	if installing then
		Install_processing = true
	
		if Meter_anim:is_playing() then
			Meter_anim:stop()
		end
		
		-- Scale the meter based on the install percent
		local new_x_scale = install_percent * LOADING_METER_END_SCALE_X
		Loading_meter:set_scale(new_x_scale, LOADING_METER_SCALE_Y)
	else
		if Install_processing then
			-- Restart the meter animation
			set_random_loading_meter()
			Install_processing = false
		end
	end
end

function ui_loading_screen_meter_done(tween_h, event_name)
	-- remove this callback
	remove_tween_end_callback( tween_h )
	
	if Install_processing then
		-- Don't start the animation again if we're installing
		return
	end
	
	set_random_loading_meter()
end