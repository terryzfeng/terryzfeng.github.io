// Get key slider
const slider = document.getElementById('key-slider');

slider.value = 0;
// Update the current slider value (each time you drag the slider handle)
slider.oninput = function() {
    if (theChuck) {
        theChuck.setInt("KEY", this.value);
    }
};