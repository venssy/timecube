var Clay = require('pebble-clay');
var clayConfig = require('./config');
var clay = new Clay(clayConfig);

var myAPIKey = '';

var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};
 
function locationSuccess(pos) {
  // Construct URL
  var url = "http://www.weather.com.cn/data/sk/101280101.html";
  /*var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + 
      '&appid=' + myAPIKey;*/
 
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      if(typeof(responseText)=="undefined"||responseText==="")
        return;
      console.log("responseText are " + responseText);
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);
 
      //if(json.cod==401) return;
      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.weatherinfo.temp /*- 273.15*/);
      console.log("Temperature is " + temperature);
 
      // Conditions
      var conditions = "rain";//json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "Temperature": temperature,
        "Conditions": conditions
      };
 
      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}
 
function locationError(err) {
  console.log("Error requesting location!");
}
 
function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}
 
// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log("PebbleKit JS ready!");
 
    // Get the initial weather
    getWeather();
  }
);
 
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log("AppMessage received!");
    getWeather();
  }                     
);
