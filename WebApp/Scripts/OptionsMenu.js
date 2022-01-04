// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';
import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'
import { getDatabase, ref, remove, child, get, update } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

const firebaseConfig = {
    apiKey: "AIzaSyB8Dzaeb7D3_QhrrD4gcDuqiSfYmesyIbw",
    authDomain: "pigate-175f8.firebaseapp.com",
    databaseURL: "https://pigate-175f8-default-rtdb.europe-west1.firebasedatabase.app",
    projectId: "pigate-175f8",
    storageBucket: "pigate-175f8.appspot.com",
    messagingSenderId: "1027851390443",
    appId: "1:1027851390443:web:50ee2a5f98c75d2a879dae"
};

const app = initializeApp(firebaseConfig);
const auth = getAuth();
const database = getDatabase();
let dbRef = ref(database);
// --------- End initialization ----------- //

/* GET URL PARAMETER FUNCTION */

function getParameterByName(name, url) {
    if (!url) url = window.location.href;
        name = name.replace(/[\[\]]/g, '\\$&');
        var regex = new RegExp('[?&]' + name + '(=([^&#]*)|&|#|$)'),
            results = regex.exec(url);
    if (!results) return null;
    if (!results[2]) return '';
    return decodeURIComponent(results[2].replace(/\+/g, ' '));
}

const gateRegId = getParameterByName("id");
let gateRegInfo = null;
/* GET URL PARAMETER FUNCTION */


/* VERIFY LOGIN DELAY*/

let delayInMilliseconds = 900;
//Set a 900 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null)
    {
        console.log("User is not logged in");
        window.location.href = "/Login.html";
    }
    else
    {
        console.log("User is logged in");
   
        //Verify if the user is valid for the Gate_Reg Id
        get(child(dbRef, `Gate_Reg/${gateRegId}`)).then((snapshot) => {
            const obj = snapshot.val();

            if(obj.email == auth.currentUser.email)
            {
                document.body.style.visibility="visible";
                gateRegInfo = obj;
                console.log("Valid user");
            }
            else
            {
                window.location.href = "/LoginSuccess/Gate.html";
            }
        }).catch((error) => {
            console.error(error);
        });

    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/




document.getElementById("isToOpenButton").onclick = function() {

    update(ref(database, `Gate/${gateRegInfo.PIgate_ID}`), {
        isToOpen: 1
        });

        //implement entry

}


document.getElementById("addPlateButton").onclick = function() {

    window.location.href = `/LoginSuccess/GateOptions/AddPlate.html?id=${gateRegId}`;

}
