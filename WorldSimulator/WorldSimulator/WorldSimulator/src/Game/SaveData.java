package Game;

import java.io.Serializable;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Collection;

public class SaveData implements Serializable {
    int playerX, playerY;
    int specialAbilityCooldown;
    int playerStrength;
    int playerInitiative;

    //we keep playerAlive in save file in order to prevent saving when dead, and becoming alive after loading
    boolean playerAlive;

    ArrayList<String> organismsClassNames = new ArrayList<>();

    ArrayList<ArrayList<Integer>> organismsCoords = new ArrayList<>();
    ArrayList<ArrayList<Integer>> organismsStats = new ArrayList<>();
}
