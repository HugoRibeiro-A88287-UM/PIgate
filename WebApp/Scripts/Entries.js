// Initialize 
import {initializeApp } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-app.js';
import {getAuth, signOut } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-auth.js'
import { getDatabase, ref, remove, child, get, update, set, push, onValue } from 'https://www.gstatic.com/firebasejs/9.6.1/firebase-database.js';

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
const dbRef = ref(database);
const dbRefEntries = ref(database,'Entry/');
const detailLength = 7; //Number of detail lables

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
const gateEntry = getParameterByName("entry");
let gateRegInfo = null; //Gate_Reg object {PIgate_Id, email}

/* GET URL PARAMETER FUNCTION */


/* VERIFY LOGIN DELAY*/

let delayInMilliseconds = 900;
//Set a 900 mseconds delay so the auth can update
setTimeout(function() {

    if(auth.currentUser == null || gateRegId == null)
    {
        console.log("Access Denied");
        window.location.href = "/LoginSuccess/Gate.html";
    }
    else
    {
        console.log("User is logged in");
        
   
        //Verify if the user is valid for the Gate_Reg Id

        get(child(dbRef, `Gate_Reg/${gateRegId}`)).then((snapshot) => {
            const obj = snapshot.val();

            if(obj.email != auth.currentUser.email)
                throw("Invalid User");

            //Get the user entries and display
            onValue(dbRefEntries, (snapshot) => {
                snapshot.forEach((childSnapshot) => {
                    const childKey = childSnapshot.key;
                    const childData = childSnapshot.val();

                    if( childData.PIgate_ID == gateRegInfo.PIgate_ID)
                    {
                        let dbRefCarReg = ref(database,`Car_Reg/${childData.Plate}|${childData.PIgate_ID}/Description`);

                        onValue(dbRefCarReg, (snapshot) => {

                            let node = [document.createElement("a")];

                            node[0].appendChild(document.createTextNode(`${childData.Plate} | ${childData.Date} | ${snapshot.val()}`));
                            node[0].href = `/LoginSuccess/GateOptions/Entries.html?id=${gateRegId}&entry=${childKey}`;
                            document.getElementById("entries").appendChild(node[0]);          

                            //If the receive entry matches with the one the the PIgate has
                            if(gateEntry == childKey)
                            {
                                //Genrate the list with detail
                                let nodeDetail = [];

                                for(let i = 0; i < detailLength ; i++)
                                    nodeDetail.push(document.createElement("a"));
                                
                                nodeDetail[0].appendChild(document.createTextNode(`${snapshot.val()}`));
                                nodeDetail[0].id = "Title"
                                nodeDetail[1].appendChild(document.createTextNode(`Plate : ${childData.Plate}`));
                                nodeDetail[2].appendChild(document.createTextNode(`Date  : ${childData.Date}`));
                                nodeDetail[3].appendChild(document.createTextNode(`Time  : ${childData.Time}`));

                                let dbRefCarReg = ref(database,`Car/${childData.Plate}`);

                                onValue(dbRefCarReg, (snapshot) => {
                                    nodeDetail[4].appendChild(document.createTextNode(`Brand : ${snapshot.val().Brand}`));
                                    nodeDetail[5].appendChild(document.createTextNode(`Model : ${snapshot.val().Model}`));
                                    nodeDetail[6].appendChild(document.createTextNode(`Colour: ${snapshot.val().Colour}`));
                                }, { onlyOnce: true });

                                for(let i = 0; i < detailLength ; i++)
                                    document.getElementById("entriesDetail").appendChild(nodeDetail[i]);  
                                
                                document.getElementById("Title").classList.add("active");
                                document.getElementById("entriesDetail").style.marginBottom = "250px";
                                document.getElementById("entriesDetail").style.visibility = "visible";

                                console.log("Detail entry done");

                            }

                        }, { onlyOnce: true });
                    }
                });
            }, { onlyOnce: true });


            //Check if a detail entry was requested


            document.body.style.visibility="visible";
            gateRegInfo = obj;
            console.log("Valid user");

        }).catch((error) => {
            console.error(error);
            window.location.href = "/LoginSuccess/Gate.html";
        });

    }

  }, delayInMilliseconds);

/* VERIFY LOGIN DELAY*/

document.getElementById("goBackButton").onclick = function() {

    window.location.href = `/LoginSuccess/GateOptions/OptionsMenu.html?id=${gateRegId}`;

}


// document.getElementById("entry0").onclick = function() {

//     window.location.href = `/LoginSuccess/GateOptions/OptionsMenu.html?id=${gateRegId}`;

// }
