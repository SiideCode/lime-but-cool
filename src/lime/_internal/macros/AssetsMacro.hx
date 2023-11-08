package lime._internal.macros;

#if macro
import haxe.crypto.BaseCode;
import haxe.io.Bytes;
import haxe.macro.Context;
import haxe.macro.Expr;
import haxe.macro.Type;
#end
#if (macro && !display)
import lime._internal.format.Base64;
import sys.io.File;
import sys.FileSystem;
#end

class AssetsMacro
{
	#if !macro
	macro public static function cacheVersion() {}
	#else
	macro public static function cacheVersion()
	{
		return macro $v{Std.int(Math.random() * 1000000)};
	}

	macro public static function embedBytes():Array<Field>
	{
		var fields = embedData(":file");

		if (fields != null)
		{
			#if !display
			var constructor = macro
				{
					var bytes = haxe.Resource.getBytes(resourceName);
					#if hl
					super(bytes.b, bytes.length);
					#else
					super(bytes.length, bytes.b);
					#end
				};

			var args = [
				{name: "length", opt: true, type: macro:Int},
				{name: "bytesData", opt: true, type: macro:haxe.io.BytesData}
			];
			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: args,
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});
			#end
		}

		return fields;
	}

	macro public static function embedBytesHL():Array<Field>
	{
		var fields = embedData(":file");

		if (fields != null)
		{
			#if !display
			var constructor = macro
				{
					var bytes = haxe.Resource.getBytes(resourceName);
					super(bytes.b, bytes.length);
				};

			var args = [
				{name: "length", opt: true, type: macro:Int},
				{name: "bytesData", opt: true, type: macro:haxe.io.BytesData}
			];
			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: args,
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});
			#end
		}

		return fields;
	}

	macro public static function embedByteArray():Array<Field>
	{
		var fields = embedData(":file");

		if (fields != null)
		{
			#if !display
			var constructor = macro
				{
					super();

					var bytes = haxe.Resource.getBytes(resourceName);
					__fromBytes(bytes);
				};

			var args = [
				{
					name: "length",
					opt: true,
					type: macro:Int,
					value: macro 0
				}
			];
			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: args,
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});
			#end
		}

		return fields;
	}

	private static function embedData(metaName:String, encode:Bool = false):Array<Field>
	{
		#if !display
		var classType = Context.getLocalClass().get();
		var metaData = classType.meta.get();
		var position = Context.currentPos();
		var fields = Context.getBuildFields();

		for (meta in metaData)
		{
			if (meta.name == metaName)
			{
				if (meta.params.length > 0)
				{
					switch (meta.params[0].expr)
					{
						case EConst(CString(filePath)):
							var path = filePath;

							if (path == "") return null;
							if (path == null) return null;

							if (!FileSystem.exists(filePath))
							{
								path = Context.resolvePath(filePath);
							}

							if (!FileSystem.exists(path) || FileSystem.isDirectory(path))
							{
								return null;
							}

							var bytes = File.getBytes(path);
							var resourceName = "__ASSET__"
								+ metaName
								+ "_"
								+ (classType.pack.length > 0 ? classType.pack.join("_") + "_" : "")
								+ classType.name;

							if (Context.getResources().exists(resourceName))
							{
								return null;
							}

							if (encode)
							{
								var resourceType = "image/png";

								if (bytes.get(0) == 0xFF && bytes.get(1) == 0xD8)
								{
									resourceType = "image/jpg";
								}
								else if (bytes.get(0) == 0x47 && bytes.get(1) == 0x49 && bytes.get(2) == 0x46)
								{
									resourceType = "image/gif";
								}

								var fieldValue = {pos: position, expr: EConst(CString(resourceType))};
								fields.push(
									{
										kind: FVar(macro:String, fieldValue),
										name: "resourceType",
										access: [APrivate, AStatic],
										pos: position
									});

								var base64 = Base64.encode(bytes);
								Context.addResource(resourceName, Bytes.ofString(base64));
							}
							else
							{
								Context.addResource(resourceName, bytes);
							}

							var fieldValue = {pos: position, expr: EConst(CString(resourceName))};
							fields.push(
								{
									kind: FVar(macro:String, fieldValue),
									name: "resourceName",
									access: [APrivate, AStatic],
									pos: position
								});

							return fields;

						default:
					}
				}
			}
		}
		#end

		return null;
	}

	macro public static function embedFont():Array<Field>
	{
		var fields = null;

		var classType = Context.getLocalClass().get();
		var metaData = classType.meta.get();
		var position = Context.currentPos();
		var fields = Context.getBuildFields();

		var path = "";
		var glyphs = "32-255";

		#if !display
		for (meta in metaData)
		{
			if (meta.name == ":font")
			{
				if (meta.params.length > 0)
				{
					switch (meta.params[0].expr)
					{
						case EConst(CString(filePath)):
							path = filePath;

							if (!sys.FileSystem.exists(filePath))
							{
								path = Context.resolvePath(filePath);
							}

						default:
					}
				}
			}
		}

		if (path != null && path != "")
		{
			var bytes = File.getBytes(path);
			var resourceName = "LIME_font_" + (classType.pack.length > 0 ? classType.pack.join("_") + "_" : "") + classType.name;

			Context.addResource(resourceName, bytes);

			for (field in fields)
			{
				if (field.name == "new")
				{
					fields.remove(field);
					break;
				}
			}

			var fieldValue = {pos: position, expr: EConst(CString(resourceName))};
			fields.push(
				{
					kind: FVar(macro:String, fieldValue),
					name: "resourceName",
					access: [APublic, AStatic],
					pos: position
				});

			var constructor = macro
				{
					super();

					__fromBytes(haxe.Resource.getBytes(resourceName));
				};

			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: [],
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});

			return fields;
		}
		#end

		return fields;
	}

	macro public static function embedImage():Array<Field>
	{
		var fields = embedData(":image");

		if (fields != null)
		{
			#if !display
			var constructor = macro
				{
					super();

					__fromBytes(haxe.Resource.getBytes(resourceName), null);
				};

			var args = [
				{
					name: "buffer",
					opt: true,
					type: macro:lime.graphics.ImageBuffer,
					value: null
				},
				{
					name: "offsetX",
					opt: true,
					type: macro:Int,
					value: null
				},
				{
					name: "offsetY",
					opt: true,
					type: macro:Int,
					value: null
				},
				{
					name: "width",
					opt: true,
					type: macro:Int,
					value: null
				},
				{
					name: "height",
					opt: true,
					type: macro:Int,
					value: null
				},
				{
					name: "color",
					opt: true,
					type: macro:Null<Int>,
					value: null
				},
				{
					name: "type",
					opt: true,
					type: macro:lime.graphics.ImageType,
					value: null
				}
			];

			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: args,
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});
			#end
		}

		return fields;
	}

	macro public static function embedSound():Array<Field>
	{
		var fields = embedData(":sound");

		if (fields != null)
		{
			#if (openfl && !display) // CFFILoader.h(248) : NOT Implemented:api_buffer_data

			var constructor = macro
				{
					super();

					var byteArray = openfl.utils.ByteArray.fromBytes(haxe.Resource.getBytes(resourceName));
					loadCompressedDataFromByteArray(byteArray, byteArray.length, forcePlayAsMusic);
				};

			var args = [
				{
					name: "stream",
					opt: true,
					type: macro:openfl.net.URLRequest,
					value: null
				},
				{
					name: "context",
					opt: true,
					type: macro:openfl.media.SoundLoaderContext,
					value: null
				},
				{
					name: "forcePlayAsMusic",
					opt: true,
					type: macro:Bool,
					value: macro false
				}
			];
			fields.push(
				{
					name: "new",
					access: [APublic],
					kind: FFun(
						{
							args: args,
							expr: constructor,
							params: [],
							ret: null
						}),
					pos: Context.currentPos()
				});
			#end
		}

		return fields;
	}
	#end
}
