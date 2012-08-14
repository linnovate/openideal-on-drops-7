Google Chart Tools - Views Integration

Google Chart Tools Views is provide a designated views format plugin named "Google Chart Tools".
After selecting this, the very first thing to do is turn on the views3 built in aggregation mode. 
Under Advanced column there a "Use aggregation" row, its should say YES.
For learning how to deal with the views3 built in aggregation, try a great screencast by Johan Falk:
http://nodeone.se/en/the-aggregation-setting-the-emmajane-episode

Now it time to add our data (fields).
The first field is representing the x-Axis (header).
Remember its have to be the first one. 
And usually it need to be set to Aggregation type: Group results together.
Many times you want to use this field by date 
(node post date or user registration date) to achieve that,
use this excellent module: drupal.org/project/views_date_format_sql

Next, lets add the numbers. this field/s will draws the graph (line points, bars, etc').
This field will mostly using one of the other "Aggregation type" functions which will returns numbers.
These numbers are points on the y-Axis (row) for each of the x-Axis points.
If the first field is a node post date with "Aggregation type" set to Group results together,
We can add node NID field with "Aggregation type" set to "Count". 

This setting will generate a chart which demonstrate the numbers of node created each day.

Sorry, but the views preview is not presenting the chart.