
class DrawShapes {
    constructor(canvasWidth, canvasHeight) {
        this.canvasWidth = canvasWidth;
        this.canvasHeight = canvasHeight;
    }
    colorRGBA(){
        const r = Math.floor(Math.random()*255);
        const g = Math.floor(Math.random()*255);
        const b = Math.floor(Math.random()*255);
        const a = Math.floor(Math.random()*256);
        let color =(r<<24)|(g<<16)|(b<<8)|a;
        color = color >>> 0; // Ensure the result is an unsigned integer
        Canvas.drawPenColor(color);
    }
    colorRGBA(color){
        color = color >>> 0; // Ensure the result is an unsigned integer
        Canvas.drawPenColor(color);
    }
    clrscr(){
        this.colorRGBA();
        Canvas.drawClearScr();
    }
    drawFRect() {
        const x = Math.floor(Math.random() * this.canvasWidth / 2);
        const y = Math.floor(Math.random() * this.canvasHeight / 2);
        const w = Math.floor(Math.random() * (this.canvasWidth-x) ); // Max width is half of canvas width
        const h = Math.floor(Math.random() * (this.canvasHeight-y)); // Max height is half of canvas height
        this.colorRGBA();
        Canvas.drawFRect(x,y,w,h);
    }
    drawFRect(x,y,w,h,c) {

        this.colorRGBA(c);
        Canvas.drawFRect(x,y,w,h);
    }
    // Method to draw a filled circle at a random position and radius
    drawFCircle() {
        const x = Math.floor(Math.random() * this.canvasWidth);
        const y = Math.floor(Math.random() * this.canvasHeight);
        // Max radius is quarter of canvas size
        const maxRadius = Math.min(Math.min(x, y),Math.min(this.canvasWidth-x, this.canvasHeight-y)); 
        const r = Math.floor(Math.random() * maxRadius) + 1; // Add 1 to ensure radius is at least 1
        this.colorRGBA();
        Canvas.drawFCircle(x,y,r);    
    }
}

// Create a DrawShapes object
const drawShapes = new DrawShapes(640, 480); // Provide canvas width and height

// Example usage:
drawShapes.clrscr();
for(i=0;i<100;i++){
    drawShapes.drawFCircle(); // Draw a random filled circle
    drawShapes.drawFRect(); // Draw a random filled circle
}

