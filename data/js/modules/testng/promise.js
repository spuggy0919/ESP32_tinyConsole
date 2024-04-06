// bug
// Function that returns a promise
function fetchData() {
    return new Promise(function(resolve, reject) {
        // Simulate fetching data asynchronously
        setTimeout(function() {
            // Simulate an error condition
            var error = new Error('Failed to fetch data');
            reject(error); // Reject the promise with an error
        }, 2000); // Simulate 2 seconds delay
    });
}

// Using the fetchData function and handling the reject case
fetchData()
    .then(function(data) {
        console.log('Data fetched successfully:', data);
    })
    .catch(function(error) {
        console.error('Error fetching data:', error.message);
    });