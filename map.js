// Initialize the map
var map = L.map('map').setView([11.444571247784047, 77.71420795909367], 10); // Set initial coordinates and zoom level

// Add a base layer to the map (e.g., OpenStreetMap)
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors'
}).addTo(map);

// Array of location coordinates and names for Bus 1
var bus1Locations = [
  [11.444571247784047, 77.71420795909367, "Katteri Pirivu"],
  [11.444167, 77.711667, "Power Office"],
  [11.4435, 77.707389, "Rajam Theatre"],
  [11.442472, 77.700778, "Aanangur Pirivu"],
  [11.44246650060728, 77.69422877126802, "KPM Bus Stand"],
  [11.441861, 77.696806, "Pallipalayam Corner"],
  [11.438778, 77.696667, "Colony Hospital"],
  [11.434861, 77.694222, "Kon Stop"],
  [11.431889, 77.691139, "Gowri Stop"],
  [11.432417, 77.694194, "Teachers Colony"],
  [11.274447333782497, 77.60709173997549, "KEC"]
];

// Array of location coordinates and names for Bus 2
var bus2Locations = [
  [11.479895, 77.847229, "Kaliyanur"],
  [11.479545, 77.837940, "AST Bunk"],
  [11.473083, 77.830850, "Chinna Goundanur"],
  [11.457611756065829, 77.82877667230511, "ICL"],
  [11.455900748507906, 77.81046590515648, "Pachampalayam"],
  [11.447315318967048, 77.74162698499637, "Valayakaranur"],
  [11.445546021770271, 77.72740929394291, "Vatamalai"],
  [11.436474, 77.714611, "Kotamedu Bypass"],
  [11.415394384413412, 77.67598995503708,"Sree Krishna Mission"],
  [11.274311, 77.605835, "Kongu Engineering College"]
];

// Array of location coordinates and names for Bus 3
var bus3Locations = [
  [11.657333, 77.822500, "Oruvapati"],
  [11.640250, 77.823139, "Chittode"],
  [11.618972, 77.803278, "Kallukadai"],
  [11.585552974409362, 77.83544886593288, "Edappadi"],
  [11.453778, 77.696389, "Cauvery Nagar"],
  [11.441833, 77.682972, "Pookadai Corner"],
  [11.274311, 77.605835, "KEC"]
];

// Create custom icons for buses
var bus1Icon = L.icon({
  iconUrl: 'static/images.png', // Placeholder bus icon URL for Bus 1
  iconSize: [32, 32],
  iconAnchor: [16, 32]
});
var bus2Icon = L.icon({
  iconUrl: 'static/images.png', // Placeholder bus icon URL for Bus 2
  iconSize: [32, 32],
  iconAnchor: [16, 32]
});
var bus3Icon = L.icon({
  iconUrl: 'static/images.png', // Placeholder bus icon URL for Bus 3
  iconSize: [32, 32],
  iconAnchor: [16, 32]
});

// Add markers and animate markers for Bus 1
addMarkersAndAnimate(bus1Locations, bus1Icon, 'red', 'Bus 1');

// Add markers and animate markers for Bus 2
addMarkersAndAnimate(bus2Locations, bus2Icon, 'blue', 'Bus 2');

// Add markers and animate markers for Bus 3
addMarkersAndAnimate(bus3Locations, bus3Icon, 'green', 'Bus 3');

// Add marker for live location
var liveMarker = L.marker([11.4289, 77.6751]).addTo(map);

// Add circle around live location with radius of 2 kilometers
var circle = L.circle([11.4289, 77.6751], {
  color: 'blue',
  fillColor: 'blue',
  fillOpacity: 0.3,
  radius: 2000,
  className: 'circle', // Adding a class to the circle for easy reference
  visible: false
}).addTo(map);

var isCircleVisible = false; // Track visibility of the circle

// Function to add markers and animate markers for a bus
function addMarkersAndAnimate(locations, icon, color, label) {
  for (var i = 0; i < locations.length; i++) {
    var marker = L.marker([locations[i][0], locations[i][1]], {icon: L.divIcon({className: 'custom-div-icon', html: '<div style="background-color: ' + color + ';" class="marker-pin"></div>'})}).addTo(map);
    marker.bindPopup(locations[i][2]); // Bind popup with location name
  }
  
  var marker = L.marker([locations[0][0], locations[0][1]], {icon: icon}).addTo(map);
  marker.bindPopup(label);

  var index = 0;
  var markerInterval = setInterval(function() {
    if (index >= locations.length - 1) {
      clearInterval(markerInterval);
      return;
    }
    var startPoint = L.latLng(locations[index]);
    var endPoint = L.latLng(locations[index + 1]);
    var route = L.Routing.control({
      waypoints: [startPoint, endPoint],
      routeWhileDragging: true,
      show: false,
      createMarker: function() {},
      fitSelectedRoutes: false
    });

    route.on('routesfound', function(e) {
      var routeCoordinates = e.routes[0].coordinates;
      var stepCount = routeCoordinates.length;
      var duration = 3000;

      var step = 0;
      var animateStep = function() {
        var progress = step / stepCount;
        var latLng = routeCoordinates[step];
        marker.setLatLng(latLng);

        step++;

        if (step < stepCount) {
          setTimeout(animateStep, duration / stepCount);
        } else {
          index++;
          if (index === locations.length - 1) {
            marker.bindPopup(label).openPopup();
          }
        }

        checkBusInRadius(marker, circle, isCircleVisible);
      };
      animateStep();
    });

    route.addTo(map);
  }, 1000);
}

// Function to check if the bus is within the circle radius
function checkBusInRadius(busMarker, circle, isCircleVisible) {
  if (isCircleVisible) {
    var busLatLng = busMarker.getLatLng();
    var circleCenter = circle.getLatLng();
    var radius = circle.getRadius();
    var distance = busLatLng.distanceTo(circleCenter);
    
    if (distance <= radius) {
      busMarker.setIcon(L.icon({
        iconUrl: 'static/img.png', // Change to active icon
        iconSize: [32, 32],
        iconAnchor: [16, 32]
      }));
    } else {
      busMarker.setIcon(L.icon({
        iconUrl: 'static/images.png', // Default bus icon
        iconSize: [32, 32],
        iconAnchor: [16, 32]
      }));
    }
  }
}

// Function to toggle the visibility of the circle
function toggleCircleVisibility() {
  var circleElement = document.querySelector('.circle');
  if (circleElement) {
    if (circleElement.style.display === 'none') {
      circleElement.style.display = 'block';
      isCircleVisible = true;
    } else {
      circleElement.style.display = 'none';
      isCircleVisible = false;
    }
  }
}

document.getElementById('toggleCircleBtn').addEventListener('click', toggleCircleVisibility);
