package Game.Entities;

import Game.Coords;
import Game.Entities.Entity;
import Game.GamePanel;

import java.awt.*;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class CollisionChecker {
    GamePanel gp;

    public CollisionChecker(GamePanel gp){
        this.gp=gp;
    }

    public boolean checkTile(Entity entity, int destX, int destY) {

        //location is out of bounds
        if(!isInWorldBounds(new Coords(destX, destY)))
            return false;

        Rectangle newBounds = new Rectangle(entity.collision.x+destX, entity.collision.x+destY,
                                            entity.collision.width, entity.collision.height);

        //find object that may collide with entity
        for(var collider : gp.gameWorld.organisms)
        {
            Rectangle colliderBounds = new Rectangle(collider.collision.x + collider.x,
                                                     collider.collision.y + collider.y,
                                                     collider.collision.width, collider.collision.height);

            //collision occured
            if(colliderBounds.intersects(newBounds))
                return entity.onCollision(collider);
        }

        //nothing collides, move is possible
        return true;
    }

    public boolean isTileFree(Coords tileCoords)
    {
        if(!isInWorldBounds(tileCoords))
            return false;

        Rectangle Bounds = new Rectangle(tileCoords.x, tileCoords.y,
                            gp.tileSize, gp.tileSize);

        //find object that may be on tile
        for(var collider : gp.gameWorld.organisms)
        {
            Rectangle colliderBounds = new Rectangle(collider.collision.x + collider.x,
                    collider.collision.y + collider.y,
                    collider.collision.width, collider.collision.height);

            //collision occured
            if(colliderBounds.intersects(Bounds))
                return false;
        }
        return true;
    }

    //returns null if there is no entity at the tile
    public Entity getTileEntity(Coords tileCoords)
    {
        if(!isInWorldBounds(tileCoords))
            return null;

        Rectangle Bounds = new Rectangle(tileCoords.x, tileCoords.y,
                                        gp.tileSize, gp.tileSize);

        //find object that may be on tile
        for(var collider : gp.gameWorld.organisms)
        {
            Rectangle colliderBounds = new Rectangle(collider.collision.x + collider.x,
                    collider.collision.y + collider.y,
                    collider.collision.width, collider.collision.height);

            //collision occured
            if(colliderBounds.intersects(Bounds))
                return collider;
        }

        return null;
    }

    public Coords[] getFreeSurroundingTiles(Coords dest) {
        Coords[] neighCells = getNeighboringCells(dest);

        List<Coords> output = new ArrayList<Coords>();

        for (var coords : neighCells) {
            if (isInWorldBounds(coords) && isTileFree(coords))
                output.add(coords);
        }

        return output.toArray(new Coords[output.size()]);
    }

    public Coords[] getNeighboringCells(Coords dest)
    {
        //get neighboring tiles
        Coords[] neighCells =
                {
                        new Coords(dest.x + gp.tileSize, dest.y),
                        new Coords(dest.x - gp.tileSize, dest.y),
                        new Coords(dest.x, dest.y + gp.tileSize),
                        new Coords(dest.x, dest.y - gp.tileSize),
                        //diagonals
                        new Coords(dest.x + gp.tileSize, dest.y + gp.tileSize),
                        new Coords(dest.x + gp.tileSize, dest.y - gp.tileSize),
                        new Coords(dest.x - gp.tileSize, dest.y + gp.tileSize),
                        new Coords(dest.x - gp.tileSize, dest.y - gp.tileSize)
                };

        ArrayList<Coords> output = new ArrayList<>();
        for (var cell : neighCells)
            if(isInWorldBounds(cell))
                output.add(cell);

        return output.toArray(new Coords[output.size()]);
    }

    public boolean isInWorldBounds(Coords coords)
    {
        if (coords.x < 0 || coords.y<0
            || coords.x > gp.worldWidth
            || coords.y > gp.worldHeight)
            return false;
        return true;

    }
}
