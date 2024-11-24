// Importera Firebase-funktioner
import { initializeApp } from "https://www.gstatic.com/firebasejs/11.0.2/firebase-app.js";
import { getDatabase, ref, onValue } from "https://www.gstatic.com/firebasejs/11.0.2/firebase-database.js";

// Din Firebase-konfiguration
const firebaseConfig = {
  apiKey: "AIzaSyA6A71DIwrLphGxwz3KI8W6aIOKiWYgdLw",
  authDomain: "i-hetaste-laget-40387.firebaseapp.com",
  databaseURL: "https://i-hetaste-laget-40387-default-rtdb.europe-west1.firebasedatabase.app",
  projectId: "i-hetaste-laget-40387",
  storageBucket: "i-hetaste-laget-40387.firebaseapp.com",
  messagingSenderId: "823859085015",
  appId: "1:823859085015:web:44a33377dbe0dd3f017a92",
};

// Initialisera Firebase
const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

// Referenser till databasen
const temperatureRef = ref(database, 'sensor/temperature');
const humidityRef = ref(database, 'sensor/humidity');

// Funktion för att uppdatera bakgrundsfärg baserat på temperatur
function updateBackground(temp) {
  const minTemp = -20; // Lägsta temperatur
  const maxTemp = 40;  // Högsta temperatur

  // Normalisera temperaturen till ett värde mellan 0 och 1
  const factor = Math.max(0, Math.min(1, (temp - minTemp) / (maxTemp - minTemp)));

  // RGB-komponenter
  const r = Math.round(130 - 60 * factor); // Från 130 (varmt) till 70 (kallt)
  const g = Math.round(210 - 130 * factor); // Från 210 (varmt) till 80 (kallt)
  const b = 255; // Blå är konstant

  // Ändra bakgrundsfärgen
  document.body.style.backgroundColor = `rgb(${r}, ${g}, ${b})`;
}

// Lyssna på temperaturuppdateringar
onValue(temperatureRef, (snapshot) => {
  const temp = snapshot.val();
  document.getElementById('temperature').textContent = temp !== null ? temp : "Ingen data";

  if (temp !== null) {
    updateBackground(temp);
  }
});

// Lyssna på luftfuktighetsuppdateringar
onValue(humidityRef, (snapshot) => {
  const humidity = snapshot.val();
  document.getElementById('humidity').textContent = humidity !== null ? humidity : "Ingen data";
});
