package lime.ui;

import lime._internal.backend.native.NativeCFFI;
import lime.app.Event;

#if !lime_debug
@:fileXml('tags="haxe,release"')
@:noDebug
#end
@:access(lime._internal.backend.native.NativeCFFI)
@:access(lime.ui.Joystick)
class Gamepad
{
	public static var devices = new Map<Int, Gamepad>();
	public static var onConnect = new Event<Gamepad->Void>();

	public var connected(default, null):Bool;
	public var guid(get, never):String;
	public var id(default, null):Int;
	public var name(get, never):String;
	public var hasLed(get, never):Bool;
	public var hasRumble(get, never):Bool;
	public var onAxisMove = new Event<GamepadAxis->Float->Void>();
	public var onButtonDown = new Event<GamepadButton->Void>();
	public var onButtonUp = new Event<GamepadButton->Void>();
	public var onDisconnect = new Event<Void->Void>();

	public function new(id:Int)
	{
		this.id = id;
		connected = true;
	}

	public static function addMappings(mappings:Array<String>):Void
	{
		#if (lime_cffi && !macro)
		#if hl
		var _mappings = new hl.NativeArray<String>(mappings.length);
		for (i in 0...mappings.length)
			_mappings[i] = mappings[i];
		var mappings = _mappings;
		#end
		NativeCFFI.lime_gamepad_add_mappings(mappings);
		#end
	}

	#if (lime_cffi && !macro && cpp && !cppia)
	public static function startRumble(id:Int, lowFreq:cpp.UInt16, highFreq:cpp.UInt16, duration:cpp.UInt32):Bool
	{
		if (devices.get(id) != null)
			return NativeCFFI.lime_gamepad_start_rumble(id, lowFreq, highFreq, duration);
		else
			return false;
	}
	#else
	public static function startRumble(id:Int, lowFreq:Int, highFreq:Int, duration:Int):Bool
	{
		return false;
	}
	#end

	#if (lime_cffi && !macro && cpp && !cppia)
	public static function setLed(id:Int, red:cpp.UInt8, green:cpp.UInt8, blue:cpp.UInt8):Bool
	{
		if (devices.get(id) != null)
			return NativeCFFI.lime_gamepad_set_led(id, red, green, blue);
		else
			return false;
	}
	#else
	public static function setLed(id:Int, red:Int, green:Int, blue:Int):Bool
	{
		return false;
	}
	#end

	@:noCompletion private static function __connect(id:Int):Void
	{
		if (!devices.exists(id))
		{
			var gamepad = new Gamepad(id);
			devices.set(id, gamepad);
			onConnect.dispatch(gamepad);
		}
	}

	@:noCompletion private static function __disconnect(id:Int):Void
	{
		var gamepad = devices.get(id);
		if (gamepad != null) gamepad.connected = false;
		devices.remove(id);
		if (gamepad != null) gamepad.onDisconnect.dispatch();
	}

	// Get & Set Methods
	@:noCompletion private inline function get_guid():String
	{
		#if (lime_cffi && !macro)
		#if hl
		return @:privateAccess String.fromUTF8(NativeCFFI.lime_gamepad_get_device_guid(this.id));
		#else
		return NativeCFFI.lime_gamepad_get_device_guid(this.id);
		#end
		#else
		return null;
		#end
	}

	@:noCompletion private inline function get_name():String
	{
		#if (lime_cffi && !macro)
		#if hl
		return @:privateAccess String.fromUTF8(NativeCFFI.lime_gamepad_get_device_name(this.id));
		#else
		return NativeCFFI.lime_gamepad_get_device_name(this.id);
		#end
		#else
		return null;
		#end
	}

	@:noCompletion private inline function get_hasLed():Bool
	{
		#if (lime_cffi && !macro)
		return NativeCFFI.lime_gamepad_has_led(this.id);
		#end
		return false;
	}

	@:noCompletion private inline function get_hasRumble():Bool
	{
		#if (lime_cffi && !macro)
		return NativeCFFI.lime_gamepad_has_rumble(this.id);
		#end
		return false;
	}
}
