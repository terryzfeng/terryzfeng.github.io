// Main file for running WebChucK

// Main Button
const title = document.getElementById('title');
const mainButton = document.getElementById('mainButton');
const stopButton = document.getElementById('stopButton');
const josh = document.getElementById('josh');

// JavaScript to read in your Chuck file
code = fetch("./hbd.ck")
    .then(response => response.text())
    .then(text => { code = text; });

// Chuck's virtual machine
let virtualMachine = -1;

// this function starts the Chuck virtual machine
async function prep() {
    await startChuck();
    await theChuckReady;
    theChuck.removeLastCode();
}

// MAIN BUTTON
mainButton.addEventListener('click', async () =>
{
        // Start WebChucK's virtual machine
        await code;
        await prep();
        theChuck.removeLastCode();
        await theChuck.runCode(code);

        // Show title
        title.innerHTML = "Happy Birthday Josh!";
        // show josh
        josh.style.display = "inline-block";

        // Update WebChucK button
        mainButton.innerHTML = "Play";
        console.log("Playing");
});

// STOP BUTTON
stopButton.addEventListener('click', async () =>
{
    // Stop WebChucK's virtual machine
    theChuck.removeLastCode();
});
