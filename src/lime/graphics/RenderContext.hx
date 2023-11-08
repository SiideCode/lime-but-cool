package lime.graphics;

import lime.app.Event;
import lime.ui.Window;

/**
	The `RenderContext` provides access to rendering for a Lime `Window`.

	Only one render context type is used at once, but when `OPENGL` or
	`OPENGLES` is the context type, compatibility contexts for other
	forms of GL (such as WebGL) may be available also.
**/
class RenderContext
{
	/**
		Additional information about the current context
	**/
	public var attributes(default, null):RenderContextAttributes;

	/**
		Access to the current Cairo render context, if available
	**/
	#if (!lime_doc_gen || native)
	public var cairo(default, null):CairoRenderContext;
	#end

	/**
		Access to the current OpenGL render API, if available
	**/
	#if (!lime_doc_gen || (native && desktop))
	public var gl(default, null):OpenGLRenderContext;
	#end

	/**
		Access to the current OpenGL ES 2.0 render API, if available
	**/
	#if (!lime_doc_gen || native)
	public var gles2(default, null):OpenGLES2RenderContext;
	#end

	/**
		Access to the current OpenGL ES 3.0 render API, if available
	**/
	#if (!lime_doc_gen || native)
	public var gles3(default, null):OpenGLES3RenderContext;
	#end

	/**
		The type of the current `RenderContext`
	**/
	public var type(default, null):RenderContextType;

	public var version(default, null):String;

	/**
		Access to the current WebGL render API, if available
	**/
	#if (!lime_doc_gen || native)
	public var webgl(default, null):WebGLRenderContext;
	#end

	/**
		Access to the current WebGL 2 render API, if available
	**/
	#if (!lime_doc_gen || native)
	public var webgl2(default, null):WebGL2RenderContext;
	#end
	public var window(default, null):Window;

	@:noCompletion private function new() {}
}
