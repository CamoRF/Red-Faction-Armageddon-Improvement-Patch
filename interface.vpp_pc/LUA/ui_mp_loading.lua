function ui_mp_loading_init()

	--ui_loading_ingame_set_next_tip()
	
	--local anims = {
		--"opening_anim",
		--"ring_animation",
		--"ring_glow_animation",
	--}
	
	--lua_batch_play_anims(anims)
end

function ui_mp_loading_cleanup()
end

function ui_mp_loading_populate_info(level_name, level_img_name, level_desc, player1, player1_img, player2, player2_img, player3, player3_img, player4, player4_img)
	local title_string = Vdo_base_object:new("title_header")
	title_string:set_text(level_name)
	
	local title_glow = Vdo_base_object:new("title_glow")
	title_glow:set_text(level_name)

	local desc_text = Vdo_base_object:new("description_text")
	desc_text:set_text(level_desc)
	
	-- Map Image
	local map_img_element = Vdo_base_object:new("map_image")
	level_img_name = level_img_name or ""
	map_img_element:set_image(level_img_name)
	if level_img_name == "" then
		map_img_element:set_visible(false)
	end
	
	-- Player Images
	player1_img = player1_img or ""
	local player1_img_element = Vdo_base_object:new("player1_image")
	player1_img_element:set_image(player1_img)
	if player1_img == "" then
		player1_img_element:set_visible(false)
	end

	player2_img = player2_img or ""
	local player2_img_element = Vdo_base_object:new("player2_image")
	player2_img_element:set_image(player2_img)
	if player2_img == "" then
		player2_img_element:set_visible(false)
	end
	
	player3_img = player3_img or ""
	local player3_img_element = Vdo_base_object:new("player3_image")
	player3_img_element:set_image(player3_img)
	if player3_img == "" then
		player3_img_element:set_visible(false)
	end

	player4_img = player4_img or ""
	local player4_img_element = Vdo_base_object:new("player4_image")
	player4_img_element:set_image(player4_img)
	if player4_img == "" then
		player4_img_element:set_visible(false)
	end

	-- Player Names
	player1 = player1 or ""
	local player1_txt_element = Vdo_base_object:new("player1_name")
	player1_txt_element:set_text(player1)
	if player1 == "" then
		local player1_bar = Vdo_base_object:new("player1_bar")
		player1_bar:set_visible(false)
	end

	player2 = player2 or ""
	local player2_txt_element = Vdo_base_object:new("player2_name")
	player2_txt_element:set_text(player2)
	if player2 == "" then
		local player2_bar = Vdo_base_object:new("player2_bar")
		player2_bar:set_visible(false)
	end
	
	player3 = player3 or ""
	local player3_txt_element = Vdo_base_object:new("player3_name")
	player3_txt_element:set_text(player3)
	if player3 == "" then
		local player3_bar = Vdo_base_object:new("player3_bar")
		player3_bar:set_visible(false)
	end

	player4 = player4 or ""
	local player4_txt_element = Vdo_base_object:new("player4_name")
	player4_txt_element:set_text(player4)
	if player4 == "" then
		local player4_bar = Vdo_base_object:new("player4_bar")
		player4_bar:set_visible(false)
	end
end

function ui_mp_loading_assets_loaded()
	vint_dataresponder_request("mp_loading_info_populate", "ui_mp_loading_populate_info")
end

--function ui_loading_ingame_set_next_tip()
	--get the next tip and duration from code
	--local new_tip, duration = rf4_loading_ingame_get_next_tip()
	
	--get the tip handle and set the new tip
	--local loading_tips = Vdo_base_object:new( "loading_tips" )
	
	--if new_tip ~= nil then
		--loading_tips:set_visible(true)
		--loading_tips:set_text( new_tip )
		
		--get the animation and tween handles
		--local tip_anim = Vdo_anim_object:new( "tip_anim" )
		--local tip_tween = Vdo_tween_object:new( "loading_tips_alpha_twn_4" )
		--local square_tween = Vdo_tween_object:new( "tip_square_group_alpha_twn_2" )
		
		--set the end event to this function so we will grab a new hint and start everything over
		--tip_tween:set_end_event( "ui_loading_ingame_tips_done" )
		
		--set the last tween's start time to the duration
		--tip_tween:set_property( "start_time", duration )
		--square_tween:set_property( "start_time", duration )
		--tip_anim:play()
	--else
		--loading_tips:set_visible(false)
	--end
--end

--function ui_loading_ingame_tips_done(tween_h, event_name)
	-- remove this callback
	--remove_tween_end_callback( tween_h )
	
	--ui_loading_ingame_set_next_tip()
--end