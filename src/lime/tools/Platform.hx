package lime.tools;

#if (haxe_ver >= 4.0) enum #else @:enum #end abstract Platform(String)
{
	var ANDROID = "android";
	var CONSOLE_PC = "console-pc";
	var IOS = "ios";
	var LINUX = "linux";
	var MAC = "mac";
	var PS3 = "ps3";
	var PS4 = "ps4";
	var TIZEN = "tizen";
	var VITA = "vita";
	var WINDOWS = "windows";
	var WIIU = "wiiu";
	var XBOX1 = "xbox1";
	var TVOS = "tvos";
	var CUSTOM = null;
}
