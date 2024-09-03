const startButton = document.getElementById('start-button');
const startBlock = document.getElementById('start-block');
const mainBlock = document.getElementById('main-block');
const page = document.getElementById('page')
const uiImage = document.getElementById('UI-image');

// initialize
const countdown = document.getElementById('countdown');
const eventDate = new Date('2024-09-01T00:00:00-07:00'); // Set event date in PST
// const eventDate = new Date('2024-09-06T00:00:00-07:00'); // Set event date in PST
let counter = setInterval(() => {
  const time = new Date();
  const diff = eventDate - time;
  // convert different into HH:MM:SS
  const hours = Math.floor(diff / 1000 / 60 / 60);
  const minutes = Math.floor(diff / 1000 / 60) % 60;
  const seconds = Math.floor(diff / 1000) % 60;
  // always display 2 digits
  countdown.innerHTML = `${hours < 10 ? '0' : ''}${hours}:${minutes < 10 ? '0' : ''}${minutes}:${seconds < 10 ? '0' : ''}${seconds}`;

  if (time >= eventDate) {
    clearInterval(counter);
    document.getElementById('count-block').classList.add('hidden'); 
    startBlock.classList.remove('hidden');
  }
}, 100);


startButton.addEventListener('click', () => {
  startBlock.classList.add('hidden');
  mainBlock.classList.remove('hidden');

  // start fireworks
  soundManager.preload().then(() => {
    init();
    audioContext.resume();
  })

  // show city
  uiImage.classList.add('outline')
  setTimeout(() => {
    uiImage.classList.add('active')
  }, 3000)

  // show msg
  setTimeout(() => {
    document.getElementById('to').style.opacity = 1;
  }, 5000);
  setTimeout(() => {
    document.getElementById('from').style.opacity = 1;
  }, 7000);
});