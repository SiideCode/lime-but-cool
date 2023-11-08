package lime.media;

#if (haxe_ver >= 4.0) enum #else @:enum #end abstract AudioContextType(String) from String to String
{
	var OPENAL = "openal";
	var CUSTOM = "custom";
}
