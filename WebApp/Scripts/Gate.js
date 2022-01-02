import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';

import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'


const firebaseConfig = {
    apiKey: "AIzaSyB8Dzaeb7D3_QhrrD4gcDuqiSfYmesyIbw",
    authDomain: "pigate-175f8.firebaseapp.com",
    databaseURL: "https://pigate-175f8-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "pigate-175f8",
    storageBucket: "pigate-175f8.appspot.com",
    messagingSenderId: "1027851390443",
    appId: "1:1027851390443:web:50ee2a5f98c75d2a879dae"
  };


// Initialize Firebase
const app = initializeApp(firebaseConfig);
const auth = getAuth();
let delayInMilliseconds = 700;

//Delete in future
console.log(auth);

//Set a 700 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null)
    {
        console.log("User is not logged in");
        window.location.href = "/Login.html";
    }
    else
    {
        console.log("User is logged in");
        let welcomeString;

        if(auth.currentUser.displayName == null)
        {
            welcomeString = "Welcome !";
        }
        else
        {
            welcomeString = "Welcome " +  auth.currentUser.displayName + " !";
        }
        
        const h1 = document.getElementById("mainHeader");

        h1.innerText = welcomeString;

    }

  }, delayInMilliseconds);







document.getElementById("logoutButton").onclick = function() {

    let number;

    const auth = getAuth();
    signOut(auth).then(() => {

        number = 0;
        window.alert("Logout Success");
        window.location.href = "/Login.html";
        
    }).catch((error) => {
        
        number++;
        console.log(number);
        window.alert("Logout Error! Try again")

    });
}
