package lime.system;

#if flash
import flash.system.Capabilities;
#end
#if (lime_cffi && !macro)
import lime._internal.backend.native.NativeCFFI;
#end

typedef LimeLocale =
{
	@:optional public var language:String;
	@:optional public var country:String;
}

@:access(lime._internal.backend.native.NativeCFFI)
class Locale
{
	public static function getLocale():Array<LimeLocale>
	{
		var locale:Array<LimeLocale> = [];

		#if (lime_cffi && !macro)
		var help:Array<Array<String>> = NativeCFFI.lime_locale_get_system_locale();
		var mama:LimeLocale;
		for (i in 0...help[0].length)
		{
			mama = {language: help[0][i], country: help[1][i]};
			locale.push(mama);
			trace(mama);
			trace(locale);
		}
		#end

		return locale;
	}
}
