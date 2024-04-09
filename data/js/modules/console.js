// Define a console object with a log function
const console = {
    id : 'console',
    log: function(...args) {
        // Print the arguments using a custom format
        var message = args.join(' '); // Concatenate arguments into a single message
        print(message); // Assuming 'print' is your output function
    },
    error: function(...args) {
        // Print the arguments using a custom format
        var message = args.join(' '); // Concatenate arguments into a single message
        print(message); // Assuming 'print' is your output function
    },
    content: function(...args) {
        var message = args.join(' '); 
        return message;
    }

};

if (typeof module === 'undefined') {  // run examples
    print("[console]console examples")

    // Usage of console.log
    console.log('Hello', 'from', 'console.log!'); // Prints: Hello from console.log!

    try {
        // Simulate an error condition
        throw new Error('This is a simulated error.');
    } catch (e) {
        // Catch and handle the error
        console.error('Error:', e.message);
    }
    console.log("Hello",Math.random());
    console.error("Oh...",123, "中文");
}

module.exports = console;
