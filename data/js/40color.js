let ctx = require('canvas');
// Function to convert a hex color code to an unsigned integer color
function hexToUintColor(hexColor) {
    // Remove the "#" symbol from the hex color code
    hexColor = hexColor.replace('#', '');

    // Parse the hex color code into individual color components
    const r = parseInt(hexColor.substring(0, 2), 16); // Red component
    const g = parseInt(hexColor.substring(2, 4), 16); // Green component
    const b = parseInt(hexColor.substring(4, 6), 16); // Blue component
    const a = parseInt(hexColor.substring(6, 8), 16); // Alpha (opacity) component

    // Combine the color components into an unsigned integer color
    const uintColor = (a << 24) | (r << 16) | (g << 8) | b;

    return uintColor >>> 0; // Ensure the result is an unsigned integer
}

// Example usage:
const vga_pallete = [
    0x000000ff, // black
	0x800000ff, // dark red
	0x008000ff, // dark green
	0x808000ff, // dark 
	0x000080ff,
	0x800080ff,
	0x008080ff,
	0xc0c0c0ff,
	0x808080ff,
	0xFF0000ff,
	0x00ff00ff,
	0xFFff00ff,
	0x0000ffff,
	0xFf00ffff,
	0x00ffffff,
	0xFFffffff
];
const uintColor = hexToUintColor('#FF00FFFF');
//清螢幕
color =vga_pallete[0]>>>0;
ctx.drawPenColor(color); //blue
ctx.drawFRect(0,0,640,480);
var i=0;
for(y=0;y<480;y+=80){
    for(x=0;x<640;x+=80){
        color=vga_pallete[i]>>>0;
        ctx.drawPenColor(color); //blue
        ctx.drawFRect(x,y,80,80); //blue
        i=(i+1)%16;

    }
}



