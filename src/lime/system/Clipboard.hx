package lime.system;

import lime._internal.backend.native.NativeCFFI;
import lime.app.Application;
import lime.app.Event;

#if !lime_debug
@:fileXml('tags="haxe,release"')
@:noDebug
#end
@:access(lime._internal.backend.native.NativeCFFI)
@:access(lime.ui.Window)
class Clipboard
{
	public static var onUpdate = new Event<Void->Void>();
	public static var text(get, set):String;
	private static var _text:String;
	@:noCompletion private static var __updated = false;

	private static function __update():Void
	{
		var cacheText = _text;
		_text = null;

		#if (lime_cffi && !macro)
		#if hl
		var utf = NativeCFFI.lime_clipboard_get_text();
		if (utf != null)
		{
			_text = @:privateAccess String.fromUTF8(utf);
		}
		#else
		_text = NativeCFFI.lime_clipboard_get_text();
		#end
		#end
		__updated = true;

		if (_text != cacheText)
		{
			onUpdate.dispatch();
		}
	}

	// Get & Set Methods
	private static function get_text():String
	{
		// Native clipboard (except Xorg) calls __update when clipboard changes.

		#if linux
		// Xorg won't call __update until we call set_text at least once.
		// Details: SDL_x11clipboard.c calls X11_XSetSelectionOwner,
		// registering this app to receive clipboard events.
		if (_text == null)
		{
			__update();

			// Call set_text while changing as little as possible. (Rich text
			// formatting will unavoidably be lost.)
			set_text(_text);
		}
		#elseif (windows || mac)
		if (!__updated)
		{
			// Lime listens for clipboard updates automatically, but if the
			// clipboard has never been updated since before the app started,
			// we need to populate the initial contents manually
			__update();
		}
		#end

		return _text;
	}

	private static function set_text(value:String):String
	{
		var cacheText = _text;
		_text = value;

		#if (lime_cffi && !macro)
		NativeCFFI.lime_clipboard_set_text(value);
		#end

		if (_text != cacheText)
		{
			onUpdate.dispatch();
		}

		return value;
	}
}
