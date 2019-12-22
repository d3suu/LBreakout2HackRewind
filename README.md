# LBreakout2HackRewind
Basic example of Windows game hacking in C++
![screenshot](https://raw.githubusercontent.com/d3suu/LBreakout2HackRewind/master/screenshot.png)

## What and why
C++ Hack for simple game (however unfinished), made in C++ with Code::Blocks IDE, for fun and somewhat profit.

## Exact game files
Download it for free, exactly this build: [portableapps.com](https://portableapps.com/apps/games/lbreakout2_portable)

## Resources
 - [Code::Blocks SetTextColor error](https://stackoverflow.com/questions/5979663/undefined-reference-to-settextcolor8)
 - [DrawText method](https://stackoverflow.com/questions/1974015/how-to-use-drawtext-to-write-text-in-a-given-window-whose-handle-is-known)
 - [Scan for key](https://stackoverflow.com/questions/6331868/using-getkeystate)
 - [Making trainer basics](http://www.rohitab.com/discuss/topic/39325-trainer-creation-basics-hack-for-fun-in-c/)

## The problems
### 1. Bonus Adresses
Turns out that bonuses don't have static addresses, and Cheat Engine cannot find pointers to them (or maybe i just don't know how to find them really).
### 2. Drawing Text
It's a bad idea to draw text like i am doing it here. Good way to do this, should be to find what library draw's text ingame (like help menu), and use direct calls to that instead.
### 3. Pause Screen Status
Everytime you move cursor out of the window, it pauses the game automatically. I was not able to find address of pause screen status, so you have to run the hack, when on pause screen in game. Also `GetKeyState` checks key status, even when not focused on window (however using [GetFocus](https://stackoverflow.com/questions/466354/how-can-i-tell-if-a-window-has-focus-win32-api) with `GetKeyState` should work fine).

## Files
 - Whole `lbreakout-trainer` is a Code::Blocks project (If compiling by hand, read `Resources: Code::Blocks SetTextColor error`)
 - `lbreakout2.CT` - Cheat Engine 7.0 file with my findings, however things like `rainbow` and `blue ball` are dynamically allocated, without pointers
 
## Specific things about hacking LBreakout2 (from my experience)
 - Bonuses are stored as 4 byte bool's
 - To activate bonus with memory, you need to write `1` to two addresses
 - You can find addresses of Bonuses by searching for `0` when they are not active, and `1` when they are active
 - Most bonuses use two bool's, while other's (like rainbow) even four bool's
 - Offset between bonuses is around 0x1C (not sure)
 - Life's are updated on death
 - There can be more life's than shown on display
 - Default paddle size is `72`
 - Maximum paddle size is `554`
 - Paddle has two values: physical and view
   - Physical represents area on which the ball can bounce
   - View represents drawn paddle on screen, so you can see how big it is
   - View pointer is static: `"lbreakout2.exe"+0006C674 + 0x20`
