import numpy as np
from bokeh.plotting import figure, curdoc
from bokeh.layouts import column
from bokeh.models import Button

points = 100*np.random.rand(3,100)
points_x = points[0].tolist()
points_y = points[1].tolist()
p = figure(x_range=(0,100), y_range=(0,100), toolbar_location=None)

circle_p = p.circle(x = points_x,
                    y = points_y, 
                    size = 20,
                    color = "navy", 
                    alpha = 0.5)
#circle_p is a GlyphrRenderer

ds = circle_p.data_source


#callback function to update circles
def callback():
    new_data = dict()
    new_points = 100*np.random.rand(3,100)
    new_data['x'] = new_points[0].tolist()
    new_data['y'] = new_points[1].tolist()
    new_data['z'] = new_points[2].tolist()
    ds.data = new_data

#Add the button widget thingie to trigger the update
button = Button(label="Update")
button.on_click(callback)

curdoc().add_root(column(p))
    
# Add the same callback as periodic
curdoc().add_periodic_callback(callback, 100)
