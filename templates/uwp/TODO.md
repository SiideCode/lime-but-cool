# TODO

## What are the plans?

UWP Windows target should be a target that guarantees that your software will be ran on Win10/11, and it should have support for all their features.
It **has to use UWP lib**, and **use of Win32 APIs isn't prohibited if UWP platform doesn't support the feature yet**.

UWP target should allow the developer to **create UWP or Win32 windows if they choose to do so**, and as much of **UWP exclusive features, or features that have weird/hard-to-use implementations in non-UWP have to have an easier alternative**.

***Ease of use on the most amount of targets possible is very important, but at the same time nobody should forbid developers from using some features just because they're not convinient to use on some targets***.

## Examples of feature alternatives

### Toast notification on Win32

**Way 1:** ***Make it work** via the method Microsoft offers (adding our app to the start menu, etc.), but I make the developer choose whether to ask the user if they want the app in the start menu to recieve notifications, or not, **making them write more code if they want to utilise the Win32 target.***

**Way 2:** ***Document** an alternative to making them via the 1st way by creating windows via the windowing system (kinda like Steam notifications), **and make an easy-to-use API to create minimalistic UWP-like toast notifications with some customisation options, using the windowing system of choice**, that is available on all 3 targets (UWP, WinRT, Win32), **is much more convinient to make work on Win32***, and works even on some targets that don't have their native notification APIs (for example Linux-based OSs).

### WebView

There's only one way out of this: *use Embeddable Chromium Framework (despite it being a whopping 168MB binary it's faster and cooler than Edge imo, but make it optional, and make it a separate .dll) or use the embeddable Edge.*

## Additions

Usage of ANGLE might be helpful with performance on windows and some other targets, though it's better to write some Direct3D, Direct2D and Vulkan bindings and renderers to make it work with less overhead, cause the translation layer definetly adds some of this overhead, which puts some additional load on the CPU, or maybe even the GPU. It might also have a chance of causing bugs (it's a huge abstraction after all), so it shouldn't be an obligation for getting the maximum graphics performance via D3D12 or Vulkan, or getting maximum compatability via D3D9.
