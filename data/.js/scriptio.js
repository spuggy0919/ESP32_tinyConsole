           // ---------------- display functions begin -------------------------------
            // VGA size graphics Function to create a dynamic card
            // display functions 
            var scalewfactor=1,scalehfactor=1;
            let canvas;
            let ctx;
            var ctxwidth;
            var scaley;
            var audio;
            // Function to create a dynamic card

            function uiInit(){
              var terminal = document.getElementById("terminal");
              var monitor = document.getElementById("consolelog");
              // canvas = document.getElementById("mycanvas");
              // ctx = canvas.getContext('2d');

              canvasAspectRatio();
              canvascheck = false;
              canvas.style.display = (consolecheck) ? 'block':'none'; 
              consolecheck = true;
              terminal.style.display = (consolecheck) ? 'block':'none'; 
              monitorcheck = false;
              monitor.style.display = (monitorcheck) ? 'block':'none';
              canvascheck = false;
              canvas.style.display = (canvascheck) ? 'block':'none';
              canvasAlign();
              // TouchEnable();
              drawclearTxt()
              // Audio
              audio = document.getElementById('myaudio'); // Get the audio element

            }

            function canvasAlign(){
              var terminal = document.getElementById("terminal");
              const content = document.querySelector('.content');
              const terminalTop  = terminal.getBoundingClientRect().top;
              const terminalLeft  = terminal.getBoundingClientRect().left;
              const terminalRight  = terminal.getBoundingClientRect().right;
              // Get the current top offset of the .content element
              const contentTop = content.getBoundingClientRect().top;
              const canvasTop = (terminalTop < 0||contentTop<0) ? 0:(terminalTop < 0) ?0:terminalTop;//Math.max(terminalTop,contentTop);
              // Set the top position of the canvas
              // Set canvas size to match textarea
              canvas.style.top = canvasTop + 'px';
              canvas.style.left = terminalLeft + 'px';
              canvas.style.right = terminalRight + 'px';
              canvas.style.bottom = (canvasTop+(terminalRight-terminalLeft)*.75) + 'px';
              canvasAspectRatio();
              drawPutImage(0,0);
            }
            window.addEventListener('scroll', function() {
              canvasAlign();
            });
            window.addEventListener('resize', function() {
              canvasAlign();
            });
            function canvasAspectRatio(){
              // createCard();
              var cards = document.getElementById("cardtiles");
              var terminal = document.getElementById("terminal");
              var ratio;
              canvas=document.getElementById("mycanvas");
              ctx=canvas.getContext('2d');
              // MonitorConsoleLog("getTransform1="+ctx.getTransform() );

              ctx.setTransform(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);

              if (cards.clientWidth<640) {
                ratio = parseFloat(canvas.width/640);
                ctx.scale(ratio,ratio);
                ctx.setTransform(ratio, 0.0, 0.0, ratio, 0.0, 0.0);
                // MonitorConsoleLog("aspectRatio="+ratio );
                // MonitorConsoleLog("aspectRatio="+(typeof ratio) );
                // MonitorConsoleLog("getTransform="+ctx.getTransform() );

                canvas.height = (canvas.width*0.75) ;
              
              }else{
                canvas.height = 480 ;
                canvas.width = 640 ;
                ratio = 1.0;
                // ctx.scale(1.0,1.0);
              }
              // ctx.scale(0.639,0.639);
                ctx.scale(ratio,ratio);

              // MonitorConsoleLog("getTransform="+ctx.getTransform() );
             
              // MonitorConsoleLog("cards.clientWidth="+cards.clientWidth );
              // MonitorConsoleLog("offsetHeight="+canvas.offsetHeight );
              // MonitorConsoleLog("width="+canvas.width );
              // MonitorConsoleLog("height="+canvas.height );
              // MonitorConsoleLog("terminalwidth="+terminal.offsetWidth );
              // MonitorConsoleLog("terminalheight="+terminal.offsetHeight ); 
           }

            // function createCard() {
            //   const cardContainer = document.getElementById('card-container');
            //   const cardtiles = document.getElementById('cardtiles');
            //   const terminal = document.getElementById('terminal');
            //     // Calculate the width and height for the card (4:3 aspect ratio)
            //     const screenWidth = window.innerWidth;
            //     const scalingFactor = screenWidth / 640;
            //     // cardtiles.style.transform = `scale(${scalingFactor})`;
            //     cardtiles.style.transformOrigin = 'top left'; // Optional: Set the transform origin

            //     // Create a card element
            //     const card = document.createElement('div');

            //     card.width = 640;
            //     card.height = 480;
            //     card.border = '10px solid #eef2c1'; // 10px border
            //     // card.style.transform = `scale(${scalingFactor})`;
            //     // card.style.transformOrigin = 'top left'; // Optional: Set the transform origin
            //     cardtiles.appendChild(card);

            //     // Create a canvas element and add it to the card
            //     canvas = document.createElement('canvas');
            //     canvas.id = "mycanvas";
            //     canvas.width = 640;// cardWidth - 10; // Subtract 20 for the border spacing
            //     canvas.height = 480;//  canvas.width * 3/4; // Subtract 20 for the border spacing
            //     // canvas.style.transform = `scale(${scalingFactor})`;
            //     // canvas.style.transformOrigin = 'top left';
            //     scalewfactor =  canvas.width/640;
            //     scalehfactor =  canvas.height/480;
            //     card.appendChild(canvas);
                
            //     // Append the card to the container
            //     cardContainer.appendChild(card);
            //     ctx = canvas.getContext('2d');

            //     // Define the rectangle parameters
            //     const x = 0; // X-coordinate of the top-left corner of the rectangle
            //     const y = 0; // Y-coordinate of the top-left corner of the rectangle
            //     const width = canvas.width; // Width of the rectangle
            //     const height = canvas.height ; // Height of the rectangle
            //     const fillColor = 'yellow'; // Fill color of the rectangle (optional)
            //     const borderColor = 'red'; // Border color of the rectangle (optional)

            //     // Draw a filled rectangle
            //     ctx.fillStyle = fillColor; // Set the fill color
            //     ctx.fillRect(x, y, width, height); // Draw the filled rectangle

            //     // Draw a stroked (bordered) rectangle (optional)
            //     ctx.strokeStyle = borderColor; // Set the border color (optional)
            //     ctx.lineWidth = 2; // Set the border line width (optional)
            //     ctx.strokeRect(x, y, width, height); // Draw the stroked rectangle (optional)

            //     ctx.fillStyle = 'blue'; //int32ToColorStyle(parseInt(color));
            //     ctx.fillRect(50, 50, 100, 100); // Draw the filled rectangle
            //     terminal.focus();

            //     TouchEnable();


            //     }
  
            // Call the createCard function to create the dynamic card
            // createCard();  
            // Define an array of functions
            var PenColor=0;
            const functionTable = [
                clearScr,
                drawFilledRect,
                drawPixel,
                drawText,
                drawHLine,
                drawVLine,
                drawLine,
                drawArc,
                drawFillArc,
                drawCircle,
                drawFillCircle,
                drawStrokeWidth,
                drawSetPenColor,
                drawRect,
                drawChar,
                drawGetImage,
                drawPutImage,
                drawSetFontSize,
                drawSetFontColor,
            ];
            
            function VideoFunc(command){
              // Example command format: "commandIndex arg1 arg2 arg3..."
              // const command = "3 Hello 100 100"; // Example "drawText" command
              // if (!canvas) {
              //   createCard();
              // }
              enableCanvas(true);
              // Parse the command to get the function index and arguments
              const commandParts = command.split(' ');
              const functionIndex = parseInt(commandParts[0]);
              const args = commandParts.slice(1);
              
              // Check if the functionIndex is valid
              if (functionIndex >= 0 && functionIndex < functionTable.length) {
                  // Call the function indirectly using the function table
                  const selectedFunction = functionTable[functionIndex];
                  selectedFunction(...args); // Spread operator to pass arguments as separate parameters
                  drawGetImage(0,0,640,480);
              } else {
                  console.log('Invalid function index');
              }
            }
            // Convert a 32-bit integer color to a CSS-style color string
            function int32ToColorStyle(int32Color) { // RGBA
               const alpha = (int32Color ) & 0xFF; // Extract the red component
              const red = (int32Color >>>24) & 0xFF; // Extract the red component
              const green = (int32Color >>> 16) & 0xFF; // Extract the green component
                const blue = (int32Color >>> 8)& 0xFF; // Extract the blue component

                // Create and return a CSS-style color string in the format "#RRGGBB"
                return `#${red.toString(16).padStart(2, '0')}${green.toString(16).padStart(2, '0')}${blue.toString(16).padStart(2, '0')}${alpha.toString(16).padStart(2, '0')}`;
            }
            // Define the functions corresponding to the function table
              function drawFilledRect( x, y, width, height) {
                // Call the drawFilledRect function to draw a filled rectangle
                const x1 = (x*scalewfactor); // X-coordinate of the top-left corner
                const y1 = (y*scalehfactor); // Y-coordinate of the top-left corner
                const width1 = (width*scalewfactor); // Width of the rectangle
                const height1 = (height*scalehfactor); // Height of the rectangle

                // ctx.fillStyle = "blue"; //int32ToColorStyle(parseInt(color));
                // ctx.fillRect(50, 50, 100, 100); // Draw the filled rectangle
                ctx.fillStyle = PenColor;
                ctx.fillRect(x1, y1, width1, height1); // Draw the filled rectangle
                console.log(`Drawing filled rectangle with color ${PenColor}, x=${x1}, y=${y1}, width=${width1}, height=${height1}`);
            }
            function drawSetFontSize( sizes) {
              ctx.font =  sizes + "px Arial Black" ;
            }
            function decimalToHexString(number)
            {
              if (number < 0)
              {
                number = 0xFFFFFFFF + number + 1;
              }

              return number.toString(16).toUpperCase();
            }
            function drawSetFontColor( fgcolor) {
              var fontcolor = int32ToColorStyle(fgcolor);
              ctx.fillStyle = fontcolor;
            } 
             var imageData;
            function drawGetImage( x, y, width, height) {
                // Call the drawFilledRect function to draw a filled rectangle
                const x1 = (x*scalewfactor); // X-coordinate of the top-left corner
                const y1 = (y*scalehfactor); // Y-coordinate of the top-left corner
                const width1 = (width*scalewfactor); // Width of the rectangle
                const height1 = (height*scalehfactor); // Height of the rectangle                // Put the image data onto the destination canvas
                imageData = ctx.getImageData(x, y, width, height);
            }
            function drawPutImage( x, y) {
                // Get the image data from the source canvas
                const x1 = (x*scalewfactor); // X-coordinate of the top-left corner
                const y1 = (y*scalehfactor); // Y-coordinate of the top-left corner
                if (typeof imageData==='undefined') return;
                ctx.putImageData(imageData, x1, y1);
            }
            function drawRect( x, y, width, height) {
                // Call the drawFilledRect function to draw a filled rectangle
                const x1 = (x*scalewfactor); // X-coordinate of the top-left corner
                const y1 = (y*scalehfactor); // Y-coordinate of the top-left corner
                const width1 = (width*scalewfactor); // Width of the rectangle
                const height1 = (height*scalehfactor); // Height of the rectangle

                // ctx.fillStyle = "blue"; //int32ToColorStyle(parseInt(color));
                // ctx.fillRect(50, 50, 100, 100); // Draw the filled rectangle
                ctx.strokeStyle = PenColor; // Stroke color of the circle
                ctx.strokeRect (x1, y1, width1, height1); // Draw the filled rectangle
                console.log(`Drawing filled rectangle with color ${PenColor}, x=${x1}, y=${y1}, width=${width1}, height=${height1}`);
            }           function drawPixel(x, y) {
                // Implement drawing a pixel with the given color at the specified coordinates
                drawFilledRect(x, y, 1, 1);
                console.log(`Drawing pixel with color ${PenColor} at x=${x}, y=${y}`);
            }

            function drawText( text, x, y, deg) {
                // Implement drawing text at the specified coordinates
                // Remove quotation marks if they exist
                const decodestr = window.atob(text); // base64 to plain string 
                var utf8Bytes = convertUtf8.stringToBytes(decodestr);
                // console.log(utf8Bytes);
                var unicodeString=convertUtf8.utf8BytesToUtf16String(utf8Bytes);
                // console.log(unicodeString);
                // ctx.fillStyle = PenColor;
                // ctx.font = '15px monospace';
                ctx.fillText(unicodeString,x,y);
                ctx.stroke();
                console.log(`Drawing text "${unicodeString}" at x=${x}, y=${y}`);
            }
            const rows = 25;
            const cols = 80;
            var textBuffer=new Array(25*80);
            var attrBuffer=new Array(25*80);
            var currentRow = 0;
            var currentCol = 0;
            const charWidth = 8; // Adjust this based on your font size and canvas size
            const charHeight = 19; // Adjust this based on your font size and canvas size
            function drawclearTxt(){
              for (let row = 0; row < rows; row++) {
                for (let col = 0; col < cols; col++) {
                  textBuffer[row*cols+col]=' ';
                  attrBuffer[row*cols+col]=0;
                }
              }
            }
            function drawScrollText() {
              ctx.fillStyle = "black";
              ctx.fillRect(0, 0, canvas.width, canvas.height);
              ctx.clearRect(0, 0, canvas.width, canvas.height);
              // Draw each character from the text buffer
              for (let row = 0; row < rows; row++) {
                for (let col = 0; col < cols; col++) {
                  drawCharAtPosXY(
                    textBuffer[row*cols+col],
                    attrBuffer[row*cols+col],
                    col*charWidth,
                    row*charHeight
                  );
                }
              }
            }
            function scrollUp() {
              // Move characters within the text buffer
              for (let row = 1; row < rows; row++) {
                for (let col = 0; col < cols; col++) {
                  textBuffer[(row - 1)*cols+col] = textBuffer[row*cols+col];
                  attrBuffer[(row - 1)*cols+col] = attrBuffer[row*cols+col];
                }
              }

              // Clear the bottom row
              for (let col = 0; col < cols; col++) {
                textBuffer[(rows - 1)*cols+col] = ' ';
                attrBuffer[(rows - 1)*cols+col] = 0;
              }

              // Redraw the canvas
              drawScrollText();
            }
            function drawChar(cin) {
              character = String.fromCharCode(cin)[0];
              if (character === '\n') {
                // Handle newline (0x0A): Move to the beginning of the next row
                currentCol = 0;
                currentRow++;
              } else if (character === '\r') {
                // Handle carriage return (0x0D): Move to the beginning of the current row
                currentCol = 0;
              } else {
                // Regular character: Add it to the buffer and move to the next column
                textBuffer[currentRow*cols+currentCol] = character;
                attrBuffer[currentRow*cols+currentCol] = PenColor;
                drawCharAtCuroser(character,PenColor);
                currentCol++;
              }
              // Check for line overflow
              if (currentCol >= cols) {
                currentCol = 0;
                currentRow++;
              }

              // Check for buffer overflow
              if (currentRow >= rows) {
                currentRow--;
                scrollUp();
                // setTimeout(scrollUp, 2000); // Scroll up after 2 seconds
              }
            }
            function drawCharAtPosXY(c,attr,x,y) {
                // Implement drawing text at the specified coordinates
                // Remove quotation marks if they exist
                ctx.fillStyle = attr;
                ctx.font = '18px monospace';
                ctx.fillText(
                    c,
                    x,
                    y 
                );
                ctx.stroke();
                console.log(`Drawing text "${c}" at x=${currentCol}, y=${currentRow}`);
            }
            function drawCharAtCuroser(c,attr) {
                // Implement drawing text at the specified coordinates
                // Remove quotation marks if they exist
                ctx.fillStyle = attr;
                ctx.font = '15px monospace';
                ctx.fillText(
                    c,
                    currentCol * charWidth,
                    currentRow * charHeight 
                );
                ctx.stroke();
                console.log(`Drawing text "${c}" at x=${currentCol}, y=${currentRow}`);
            }
            function drawHLine(y, x1, x2) { // Hline x1,x2 at y
                // Implement drawing a horizontal line from (x1, y) to (x2, y)
                drawLine(x1, y, x2, y);
                console.log(`Drawing horizontal line from x=${x1} to x=${x2}, y=${y}`);
            }

            function drawVLine(x, y1, y2) { // Vline Y1,y2 at x
            // Plot a line from (x, y) to (x1, y1) with color
                drawLine(x, y1, x, y2);
                console.log(`Drawing vertical line from x=${x}, y=${y1} to y=${y2}`);
            }

            function drawLine(x1, y1, x2, y2) {
                // Implement plotting coordinates from (x1, y1) to (x2, y2)
                ctx.beginPath();
                ctx.moveTo(x1*scalehfactor, y1*scalehfactor);
                ctx.lineTo(x2*scalehfactor, y2*scalehfactor);
                ctx.strokeStyle = PenColor;
                ctx.stroke();
                console.log(`Plotting coordinates from x=${x1}, y=${y1} to x=${x2}, y=${y2}`);
            }
            function drawSetPenColor(color)
            {
               PenColor =int32ToColorStyle(color);
            }
            function drawStrokeWidth(width)
            {          
               ctx.lineWidth = width; // Line width (optional)
            }
            function drawCircle( x, y, radius)
            {
              ctx.beginPath(); // Begin a new path
              ctx.arc(x, y, radius, 0, 2 * Math.PI); // Define the circle
              ctx.strokeStyle = PenColor; // Stroke color of the circle
              // ctx.lineWidth = 2; // Line width (optional)
              ctx.stroke(); // Stroke the circle outline
              ctx.closePath(); // Close the path
            }
            function drawFillCircle( x, y, radius)
            {
              ctx.beginPath(); // Begin a new path
              ctx.arc(x, y, radius, 0, 2 * Math.PI); // Define the circle
              ctx.fillStyle = PenColor; // Stroke color of the circle
              ctx.fill(); // Stroke the circle outline
              ctx.closePath(); // Close the path
            }
            function drawArc( x, y, radius, from, to )
            {
              ctx.beginPath(); // Begin a new path
              ctx.arc(x, y, radius, from, to); // Define the circle
              ctx.strokeStyle = PenColor; // Stroke color of the circle
              // ctx.lineWidth = 2; // Line width (optional)
              ctx.stroke(); // Stroke the circle outline
              ctx.closePath(); // Close the path
            }
            function drawFillArc( x, y, radius, from, to )
            {
              ctx.beginPath(); // Begin a new path
              ctx.arc(x, y, radius, from, to); // Define the circle
              ctx.fillStyle = PenColor; // Stroke color of the circle
              // ctx.lineWidth = 2; // Line width (optional)
              ctx.stroke(); // Stroke the circle outline
              ctx.closePath(); // Close the path
            }
            function clearScr()
            {
                  const canvas = document.getElementById('mycanvas');
                  ctx.fillStyle = PenColor;
                  ctx.fillRect(0, 0, canvas.width, canvas.height);
            }            // ---------------- display functions ends -------------------------------
            // ---------------- Audio  functions Begins -------------------------------
            const AudiofunctionTable = [
                playAudio,
                pauseAudio,
                stopAudio,
                seekForward,
                seekBackward,
                changeSong, // 5
            ]; 
            function AudioFunc(command){


              // Parse the command to get the function index and arguments
              const commandParts = command.split(' ');
              const functionIndex = parseInt(commandParts[0]);
              const args = commandParts.slice(1);

              // Check if the functionIndex is valid
              if (functionIndex >= 0 && functionIndex < functionTable.length) {
                  // Call the function indirectly using the function table
                  const selectedFunction = AudiofunctionTable[functionIndex];
                  selectedFunction(...args); // Spread operator to pass arguments as separate parameters
              } else {
                  console.log('Invalid function index');
              }
            }
            // Play the audio
            function changeSong(filename) {
              audio.src = filename;
            }
            // Play the audio
            function playAudio() {
              audio.play();
            }

            // Pause the audio
            function pauseAudio() {
              audio.pause();
            }

            // Stop the audio (pause and reset to the beginning)
            function stopAudio() {
              audio.pause();
              audio.currentTime = 0;
            }

            // Seek forward in the audio by a specified number of seconds
            function seekForward(seconds) {
              audio.currentTime += seconds;
            }

            // Seek backward in the audio by a specified number of seconds
            function seekBackward(seconds) {
              audio.currentTime -= seconds;
            }
            // ---------------- Audio  functions ends -------------------------------

