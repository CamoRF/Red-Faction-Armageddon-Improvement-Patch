function var_to_string(var)
	if type(var) == "table" then
		return "{table}"
	elseif var == nil then
		return "nil"
	elseif var == true then
		return "true"
	elseif var == false then
		return "false"
	else
		return var
	end
end

--Easy Start Anim
function lua_play_anim(anim_h, start_time_offset, doc)
	if start_time_offset == nil then
		start_time_offset = 0
	end

	local start_time = vint_get_time_index(doc) + start_time_offset

	vint_set_property(anim_h, "is_paused", false)
	vint_set_property(anim_h, "start_time", start_time)
end

--Easy Play tween method
function lua_play_tween(tween_handle, target_handle, start_time_offset)
	vint_set_property(tween_handle, "target_handle",	target_handle)
	vint_set_property(tween_handle, "start_time",		vint_get_time_index() + start_time_offset)
	vint_set_property(tween_handle, "state",				"running")
end

function lua_batch_play_anims(anim_name_list)
	for i, name in pairs(anim_name_list) do
		local anim = Vdo_anim_object:new(name)
		anim:play()
	end
end

function lua_batch_stop_anims(anim_name_list)
	for i, name in pairs(anim_name_list) do
		local anim = Vdo_anim_object:new(name)
		anim:stop()
	end
end

--Sets actual size of the element. This is independent of the parent scaler values.
function element_set_actual_size(element_h, width, height)
	local current_width, current_height = vint_get_property(element_h, "unscaled_size")
	if current_width == nil or current_height == nil then
		debug_print("vint", "Can't set actual size, element handle: " .. element_h .. " does not exist.\n")
		return
	end
	local scale_x = width/current_width
	local scale_y = height/current_height
	vint_set_property(element_h, "scale", scale_x, scale_y)
end

--Returns actual size of the element, independent of the parent scaler values.
function element_get_actual_size(element_h)
	local width, height = vint_get_property(element_h, "unscaled_size")
	local scale_x, scale_y = vint_get_property(element_h, "scale")
	width = width * scale_x
	height = height * scale_y
	return width, height
end

--Returns the NW and SE corners of the element's 2D bounding box (scaled based on resolution)
function element_get_bbox(element_handle)
	return vint_get_bbox(element_handle)
end

-- Helper function for list options that wraps integral indexes around 1 and a max index value
--
-- current_idx: current value
-- direction: value to add/subtract
-- max_idx: maximum index value, inclusive
function wrap_index(current_idx, direction, max_idx)
	return wrap_value(current_idx + direction, 1, max_idx + 1)
end

-- Cap a value to the given min and max
-- val: value to cap
-- min_val: minimum value, inclusive
-- max_val: maximum value, inclusive
function cap_value(val, min_val, max_val)
	if val < min_val then
		return min_val
	elseif val > max_val then
		return max_val
	end
	
	return val
end

-- Wrap a value around the given min and max (where the value of the min and max are considered equal, like 0 and 360 degrees)
-- val: value to wrap
-- min_val: minimum value, inclusive
-- max_val: maximum value, NON-inclusive (will wrap to minimum value)
function wrap_value(val, min_val, max_val)
	local val0 = val - min_val
	local base = max_val - min_val
	
	return (val0 % base) + min_val
end

-- Linear interpolation between a low and high value with a given percent
-- low: minimum value
-- high: maximum value
-- percent: percent to interpolate between the low and high values 
function get_lerp(low, high, percent)
	return low + ((high - low) * percent)
end

-- Linear interpolation for y in a range between low_y and high_y, given a current value x and a range low_x and high_x
-- x: input value
-- low_x: input min value
-- high_x: input max value
-- low_y: output min value
-- high_y: output max value
function get_lerp_from_range(x, low_x, high_x, low_y, high_y)
	local percent = (x - low_x) / (high_x - low_x)
	return get_lerp(low_y, high_y, percent)
end

-- Helper to stall a thread while the responder works
function vint_dataresponder_request(data_responder_name, callback_function_name, max_records_to_return, ...)
	max_records_to_return = max_records_to_return or 0

	vint_internal_dataresponder_request(data_responder_name, callback_function_name, max_records_to_return, ...)
	while(vint_dataresponder_finished(data_responder_name) ~= true) do
		thread_yield()
		vint_internal_dataresponder_request(data_responder_name, callback_function_name, max_records_to_return, ...)
	end
end
