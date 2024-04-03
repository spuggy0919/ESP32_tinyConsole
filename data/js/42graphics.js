
let ctx = require('canvas');
class DrawShapes {
    constructor(width, height) {
        this.width = width;
        this.height = height;
    }
    colorRGBAr(){
        let r = Math.floor(Math.random()*255);
        let g = Math.floor(Math.random()*255);
        let b = Math.floor(Math.random()*255);
        let a = Math.floor(Math.random()*256);
        let color =(r<<24)|(g<<16)|(b<<8)|a;
        color = color >>> 0; // Ensure the result is an unsigned integer
        ctx.drawPenColor(color);
    }
    colorRGBA(color){
        color = color >>> 0; // Ensure the result is an unsigned integer
        ctx.drawPenColor(color);
    }
    clrscr(){
        this.colorRGBAr();
        ctx.drawFRect(0,0,640,480);
    }
    drawFRectr() {
        let x = Math.floor(Math.random() * this.width / 2);
        let y = Math.floor(Math.random() * this.height / 2);
        let w = Math.floor(Math.random() * (this.width-x) ); // Max width is half of canvas width
        let h = Math.floor(Math.random() * (this.height-y)); // Max height is half of canvas height
        this.colorRGBAr();
        ctx.drawFRect(x,y,w,h);
    }
    drawFRect(x,y,w,h,c) {

        this.colorRGBA(c);
        ctx.drawFRect(x,y,w,h);
    }
    // Method to draw a filled circle at a random position and radius
    drawFCircle() {
        let x = Math.floor(Math.random() * this.width);
        let y = Math.floor(Math.random() * this.height);
        // Max radius is quarter of canvas size
        let maxRadius = Math.min(Math.min(x, y),Math.min(this.width-x, this.height-y)); 
        let r = Math.floor(Math.random() * maxRadius) + 1; // Add 1 to ensure radius is at least 1
        this.colorRGBAr();
        ctx.drawFCircle(x,y,r);    
    }
}

// Create a DrawShapes object
const draw = new DrawShapes(640, 480); // Provide canvas width and height

// Example usage:
draw.clrscr();
for(i=0;i<100;i++){
    draw.drawFCircle(); // Draw a random filled circle
    draw.drawFRectr(); // Draw a random filled rect
}

