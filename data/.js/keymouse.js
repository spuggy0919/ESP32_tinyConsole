const mycanvas = document.getElementById('mycanvas');
const textarea = document.getElementById('terminal');
let canvasActive = false;
// Initial setup
const switchCheckbox = document.getElementById('switchTouch');
switchToTextarea();

function switchToCanvas() {
  canvasActive = true;
//   mycanvas.style.pointerEvents = 'auto';
//   textarea.style.pointerEvents = 'none';

  // Add canvas-related event listeners here
  mycanvas.addEventListener('touchstart', handleTouchStart);
  mycanvas.addEventListener('touchmove', handleTouchMove);
  mycanvas.addEventListener('touchend', handleTouchEnd);
  // Add mouse event listeners
  mycanvas.addEventListener('mousedown', handleMouseDown);
  mycanvas.addEventListener('mousemove', handleMouseMove);
  mycanvas.addEventListener('mouseup', handleMouseUp);  
  // Add more canvas-related event listeners as needed
}

function switchToTextarea() {
  canvasActive = false;
//   mycanvas.style.pointerEvents = 'none';
//   textarea.style.pointerEvents = 'auto';

  // Remove canvas-related event listeners here

    // Add touch event listeners
    mycanvas.removeEventListener('touchstart', handleTouchStart);
    mycanvas.removeEventListener('touchmove', handleTouchMove);
    mycanvas.removeEventListener('touchend', handleTouchEnd);

    // Add mouse event listeners
    mycanvas.removeEventListener('mousedown', handleMouseDown);
    mycanvas.removeEventListener('mousemove', handleMouseMove);
    mycanvas.removeEventListener('mouseup', handleMouseUp);  
        // Remove more canvas-related event listeners as needed
}





document.getElementById('switchTouch').addEventListener('change', function() {
  if (switchCheckbox.checked) {
    switchToCanvas();
    textarea.style.zIndex = 1;
    mycanvas.style.zIndex = 2;  
    mycanvas.style.display = "block"
  } else {
    switchToTextarea();
    textarea.style.zIndex = 2;
    mycanvas.style.zIndex = 1;
    mycanvas.style.display = "none"
  }
  mycanvas.style.zIndex = 1;

});
function updateCanvasPosition() {
    // Set canvas size to match textarea
    mycanvas.width = textarea.clientWidth;
    mycanvas.height = textarea.clientWidth*0.75;

    // Set canvas position to match textarea
    mycanvas.style.top = textarea.offsetTop + 'px';
    mycanvas.style.left = textarea.offsetLeft + 'px';

    // 
    canvasAspectRatio();
}
// Initial positioning
updateCanvasPosition();

// Add event listener for window resize
window.addEventListener('resize', updateCanvasPosition);

// Handle touch start event
function handleTouchStart(event) {
    event.preventDefault();
    const touch = event.touches[0]; // Get the first touch (assuming single touch)
    const x = touch.clientX - mycanvas.getBoundingClientRect().left;
    const y = touch.clientY - mycanvas.getBoundingClientRect().top;

    wsSentTouch(1,x,y);
}


// Handle touch move event
function handleTouchMove(event) {
    event.preventDefault();
    const touch = event.touches[0];
    const x = touch.clientX - mycanvas.getBoundingClientRect().left;
    const y = touch.clientY - mycanvas.getBoundingClientRect().top;

 // ctx.clearRect(0, 0, canvas.width, canvas.height);
    // ctx.drawImage(cursorImage, x, y, 32, 32); // to low Adjust width and height as needed

    wsSentTouch(2,x,y);
}

// Handle touch end event
function handleTouchEnd(event) {
    event.preventDefault();

    const x = touch.clientX - mycanvas.getBoundingClientRect().left;
    const y = touch.clientY - mycanvas.getBoundingClientRect().top;

    wsSentTouch(0,x,y);
}

// Handle mouse down event
function handleMouseDown(event) {
    event.preventDefault();
    const x = event.clientX - mycanvas.getBoundingClientRect().left;
    const y = event.clientY - mycanvas.getBoundingClientRect().top;

    wsSentMouse(1,x,y);
}

// Handle mouse move event
function handleMouseMove(event) {
    event.preventDefault();
    const x = event.clientX - mycanvas.getBoundingClientRect().left;
    const y = event.clientY - mycanvas.getBoundingClientRect().top;

    wsSentMouse(2,x,y);
}

// Handle mouse up event
function handleMouseUp(event) {
    event.preventDefault();
    const x = event.clientX - mycanvas.getBoundingClientRect().left;
    const y = event.clientY - mycanvas.getBoundingClientRect().top;
    wsSentMouse(0,x,y);
    // Handle mouse up logic
}