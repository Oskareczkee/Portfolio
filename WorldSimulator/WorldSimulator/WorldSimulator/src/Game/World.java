package Game;

import java.awt.*;
import java.util.ArrayList;
import java.util.List;

public class World{
    public List<Organism> organisms = new ArrayList<Organism>();
    public void addOrganism(Organism organism)
    {
        organisms.add(organism);
    }

    public void draw(Graphics2D g2)
    {
        for (var organism: organisms) {
            organism.draw(g2);
        }
    }
}
