# Scroll Test ROM

A ROM I put together in SGDK to have scroll tests I
can quickly control and make save states to compare
against drawing logic in my gssex project, which
you will (eventually) be able to find on my github.

Code is probably not great for C; I haven't done C
in ages. For example, I couldn't get function
pointers to work after 30 minutes of messing around
so I relented to using a switch block with function
calls instead.

## Compiling
Requires [SGDK](https://github.com/Stephane-D/SGDK) to be setup in your environment.

I was following a set of tutorials to learn the SGDK
basics that used VSCode with the Genesis Code plug-in,
however, you can probably adapt it to other SGDK templates
without too much trouble.

## Controls
+ Left/Right - sets the x offset in the current test
+ Up/Down - sets the y offset in the current test
+ Button A - cycle to next test
+ Button B - reset any offsets in current test
+ Button C - for test with vertical scroll column, cycle the column

## Tests
### LINE TEST
Hor. Scroll - Line
Vert. Scroll - Column

Lines 8-15 are shifted by the current offset which each line having an
additiona increment of 1. Squares turn into rhombus like shapes.

### LINE PLANE TEST
Hor. Scroll - Line
Vert. Scroll - Plane

Similar to above, but vertical scroll is whole plane instead of column.

### TILE TEST
Hor. Scroll - Tile
Vert. Scroll - Column

Tile row 1 if offset by current x offset.

### SCREEN TEST
Hor. Scroll - Plane
Vert. Scroll - Column

### FULL TEST
Hor. Scoll - Plane
Vert. Scroll - Plane