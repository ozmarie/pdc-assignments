/** Graph.java
 * Used to draw graphs for the ForestFire project
 * using two variables (probabilities on the x-axis, 
 * and some other measured variable on the y-axis)
 * 
 * Based on code from https://www.javatpoint.com/java-plot
 */

import java.awt.*;
import javax.swing.*;
import java.awt.geom.*;

public class Graph extends JPanel {
    int margin = 60;
    double[] probabilities;
    double[] variables;

    public Graph(double[] probs, double[] vars) {
        this.probabilities = probs;
        this.variables = vars;
    }

    protected void paintComponent(Graphics grf) {
        super.paintComponent(grf);
        Graphics2D graph = (Graphics2D)grf;

        //Sets the value of a single preference for the rendering algorithms.  
        graph.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);  
          
        // get width and height  
        int width = getWidth();  
        int height = getHeight();  
          
        // draw graph axes
        graph.draw(new Line2D.Double(margin, margin, margin, height-margin));  
        graph.draw(new Line2D.Double(margin, height-margin, width-margin, height-margin));  
          
        //find scale for plotting points  
        double x_scale = (double)(width-2*margin);  
        double y_scale = (double)(height-2*margin)/getMax();
          
        //set color for points  
        graph.setPaint(Color.BLUE);  
          
        // set points to the graph  
        for(int i=0; i<this.probabilities.length; ++i){  
            double x1 = margin+(this.probabilities[i]*x_scale);  
            double y1 = height-margin-(this.variables[i]*y_scale);  
            graph.fill(new Ellipse2D.Double(x1-2, y1-2, 4, 4));  
        }  
    }

    //create getMax() method to find maximum value  
    private double getMax(){  
        double max = -Double.MAX_VALUE;  
        for(int i=0; i<variables.length; ++i){  
            if(variables[i]>max)  
                max = variables[i];  
             
        }  
        return max;  
    }         
    
}