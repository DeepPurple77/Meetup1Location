var locationOptions = { "timeout": 15000, "maximumAge": 60000 };
var window;

function appMessageAck(e) {
    console.log("options sent to Pebble successfully");
}

function appMessageNack(e) {
    console.log("options not sent to Pebble: " + e.error);
	console.log("Transaction ID: " + e.data.transactionId);
}

function locationSuccess(pos) {
  var location = pos.coords;
  
  var latStr = '' + location.latitude;
  latStr = latStr.substring(0,8);
	
  var longStr = '' + location.longitude;
  longStr = longStr.substring(0,8);

  var locString = (latStr + " " + longStr);
  
  Pebble.sendAppMessage({
	  "coords":locString
  }, appMessageAck, appMessageNack);

}

function locationError(err) {
  console.warn('location error');
  Pebble.sendAppMessage({
	  "coords":"Error"
  }, appMessageAck, appMessageNack);
}

Pebble.addEventListener("ready", function() {
	console.log("Pebble JS ready");
	Pebble.sendAppMessage({
		"ready":1
	});
});

Pebble.addEventListener("appmessage",
                        function(e) {
							if (e.payload.get_location) {
								window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
							}
});