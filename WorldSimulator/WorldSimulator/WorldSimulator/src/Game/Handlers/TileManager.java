package Game.Handlers;

import Game.GamePanel;
import Game.Tile;

import javax.imageio.ImageIO;
import java.awt.*;
import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

public class TileManager {
    GamePanel gp;
    Tile[] tile;
    int mapTileNum[][];

    public TileManager(GamePanel gp)
    {
        this.gp=gp;
        tile = new Tile[10];
        mapTileNum = new int [gp.maxWorldCol][gp.maxWorldRow];
        getTileImage();
        loadMap("Maps/map01.txt");
    }

    public void getTileImage(){
        try {
            tile[0] = new Tile();
            tile[0].img= ImageIO.read(getClass().getClassLoader().getResourceAsStream("Tiles/grass.png"));
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }

    public void loadMap(String mapPath)
    {
        try{
            InputStream is = getClass().getClassLoader().getResourceAsStream(mapPath);
            BufferedReader br = new BufferedReader(new InputStreamReader(is));

            int col =0;
            int row=0;
            String line;

            while(col < gp.maxWorldCol && row<gp.maxWorldRow && (line=br.readLine())!=null)
            {
                String numbers[] = line.split(" ");

                while (col < gp.maxWorldCol && col<numbers.length)
                {
                    int num = Integer.parseInt(numbers[col]);

                    mapTileNum[col][row]=num;
                    col++;
                }
                col=0;
                row++;
            }
            br.close();
        }
        catch (Exception e){
            e.printStackTrace();
        }
    }
    public void draw(Graphics2D g2){
        int worldCol=0, worldRow=0;

        while(worldCol < gp.maxWorldCol && worldRow < gp.maxWorldRow){
            int tileNum = mapTileNum[worldCol][worldRow];

            int worldX = worldCol * gp.tileSize;
            int worldY = worldRow * gp.tileSize;
            int ScreenX = worldX - gp.player.x + gp.player.ScreenX;
            int ScreenY = worldY - gp.player.y + gp.player.ScreenY;



            //render only tiles within the boundary of screen
            //render 1 tile more, to remove filckering when rendering
            if(worldX + gp.tileSize > gp.player.x - gp.player.ScreenX&&
                    worldX - gp.tileSize < gp.player.x + gp.player.ScreenX&&
                    worldY + gp.tileSize > gp.player.y -gp.player.ScreenY&&
                    worldY - gp.tileSize < gp.player.y + gp.player.ScreenY)
                g2.drawImage(tile[tileNum].img, ScreenX,ScreenY,gp.tileSize, gp.tileSize,null);

            worldCol++;
            if(worldCol==gp.maxWorldCol){
                worldCol =0;
                worldRow++;
            }

        }
    }
}
