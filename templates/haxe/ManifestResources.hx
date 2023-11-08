package;

import haxe.io.Bytes;
import lime.utils.AssetBundle;
import lime.utils.AssetLibrary;
import lime.utils.AssetManifest;
import lime.utils.Assets;

#if sys
import sys.FileSystem;
#end

#if disable_preloader_assets
@:dox(hide) class ManifestResources {
	public static var preloadLibraries:Array<Dynamic>;
	public static var preloadLibraryNames:Array<String>;
	public static var rootPath:String;

	public static function init (config:Dynamic):Void {
		preloadLibraries = new Array ();
		preloadLibraryNames = new Array ();
	}
}
#else
@:access(lime.utils.Assets)


@:keep @:dox(hide) class ManifestResources {


	public static var preloadLibraries:Array<AssetLibrary>;
	public static var preloadLibraryNames:Array<String>;
	public static var rootPath:String;


	public static function init (config:Dynamic):Void {

		preloadLibraries = new Array ();
		preloadLibraryNames = new Array ();

		rootPath = null;

		if (config != null && Reflect.hasField (config, "rootPath")) {

			rootPath = Reflect.field (config, "rootPath");

			if(!StringTools.endsWith (rootPath, "/")) {

				rootPath += "/";

			}

		}

		if (rootPath == null) {

			#if (ios || tvos)
			rootPath = "assets/";
			#elseif android
			rootPath = "";
			#elseif console
			rootPath = lime.system.System.applicationDirectory;
			#else
			rootPath = "./";
			#end

		}

		#if (openfl  && !display)
		::if (assets != null)::::foreach assets::::if (type == "font")::openfl.text.Font.registerFont (__ASSET__OPENFL__::flatName::);
		::end::::end::::end::
		#end

		var data, manifest, library, bundle;

		#if kha

		::manifest::
		library = AssetLibrary.fromManifest (manifest);
		Assets.registerLibrary ("::library::", library);

		if (library != null) preloadLibraries.push (library);
		else preloadLibraryNames.push ("::library::");

		#else

		::if (assets != null)::::foreach assets::::if (type == "manifest")::::if (embed)::data = '::data::';
		manifest = AssetManifest.parse (data, rootPath);
		library = AssetLibrary.fromManifest (manifest);
		Assets.registerLibrary ("::library::", library);
		::else::Assets.libraryPaths["::library::"] = rootPath + "::resourceName::";
		::end::::end::::if (type == "bundle")::::if (embed)::
		bundle = AssetBundle.fromBytes (new __ASSET__::flatName:: ());
		library = AssetLibrary.fromBundle (bundle);
		Assets.registerLibrary ("::library::", library);
		::else::Assets.bundlePaths["::library::"] = rootPath + "::resourceName::";
		::end::::end::::end::::end::

		::foreach libraries::::if (preload)::library = Assets.getLibrary ("::name::");
		if (library != null) preloadLibraries.push (library);
		else preloadLibraryNames.push ("::name::");
		::end::::end::

		#end

	}


}


#if kha

::images::

#else

#if !display
#if (desktop || cpp)

::if (assets != null)::::foreach assets::::if (embed)::@:keep ::if (type == "image")::@:image("::sourcePath::") @:noCompletion #if display private #end class __ASSET__::flatName:: extends lime.graphics.Image {}
::elseif (type == "sound")::@:file("::sourcePath::") @:noCompletion #if display private #end class __ASSET__::flatName:: extends haxe.io.Bytes {}
::elseif (type == "music")::@:file("::sourcePath::") @:noCompletion #if display private #end class __ASSET__::flatName:: extends haxe.io.Bytes {}
::elseif (type == "font")::@:font("::sourcePath::") @:noCompletion #if display private #end class __ASSET__::flatName:: extends lime.text.Font {}
::else::@:file("::sourcePath::") @:noCompletion #if display private #end class __ASSET__::flatName:: extends haxe.io.Bytes {}
::end::::end::::end::::end::
::if (assets != null)::::foreach assets::::if (!embed)::::if (type == "font")::@:keep @:noCompletion #if display private #end class __ASSET__::flatName:: extends lime.text.Font { public function new () { ::if (targetPath != null)::__fontPath = ManifestResources.rootPath + "::targetPath::";::else::::if (library != null)::__fontID = "::library:::::id::";::else::__fontID = "::id::";::end::::end:: ::if (fontName)::name = "::fontName::";::end:: super (); }}
::end::::end::::end::::end::

#else

::if (assets != null)::::foreach assets::::if (type == "font")::@:keep @:expose('__ASSET__::flatName::') @:noCompletion #if display private #end class __ASSET__::flatName:: extends lime.text.Font { public function new () { #if !html5 __fontPath = "::targetPath::"; #else ascender = ::ascender::; descender = ::descender::; height = ::height::; numGlyphs = ::numGlyphs::; underlinePosition = ::underlinePosition::; underlineThickness = ::underlineThickness::; unitsPerEM = ::unitsPerEM::; #end::if (fontName):: name = "::fontName::";::end:: super (); }}
::end::::end::::end::

#end

#if (openfl)

::if (assets != null)::::foreach assets::::if (type == "font")::@:keep @:expose('__ASSET__OPENFL__::flatName::') @:noCompletion #if display private #end class __ASSET__OPENFL__::flatName:: extends openfl.text.Font { public function new () {::if (embed):: __fromLimeFont (new __ASSET__::flatName:: ());::else:: ::if (targetPath != null)::__fontPath = ManifestResources.rootPath + "::targetPath::";::else::::if (library != null)::__fontID = "::library:::::id::";::else::__fontID = "::id::";::end::::end::::if (fontName):: name = "::fontName::";::end::::end:: super (); }}
::end::::end::::end::

#end
#end

#end

#end