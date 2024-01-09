/*
 * The MIT License (MIT)
 *
 * FXGL - JavaFX Game Library
 *
 * Copyright (c) 2015-2017 AlmasB (almaslvl@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.almasb.fxglgames.pong;

import com.almasb.fxgl.animation.Interpolators;
import com.almasb.fxgl.app.ApplicationMode;
import com.almasb.fxgl.app.GameApplication;
import com.almasb.fxgl.app.GameSettings;
import com.almasb.fxgl.core.math.FXGLMath;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.level.Level;
import com.almasb.fxgl.entity.level.text.TextLevelLoader;
import com.almasb.fxgl.net.*;
import com.almasb.fxgl.physics.CollisionHandler;
import com.almasb.fxgl.physics.HitBox;
import com.almasb.fxgl.time.TimerAction;
import com.almasb.fxgl.ui.UI;
import javafx.geometry.Point2D;
import javafx.scene.paint.Color;
import javafx.scene.text.Text;
import javafx.util.Duration;
import org.jetbrains.annotations.NotNull;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import static com.almasb.fxgl.dsl.FXGL.*;
import static com.almasb.fxglgames.pong.Crypto.*;

/**
 * An implementation of Bomberman Server based on the codebase provided by Almas Baimagambetov.
 *
 *
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public class BombermanApp extends GameApplication implements MessageHandler<String> {

    @Override
    protected void initSettings(GameSettings settings) {
        settings.setTitle("Bomberman Server");
        settings.setVersion("1.0");
        settings.setFontUI("pong.ttf");
        settings.setTicksPerSecond(60);
        settings.setApplicationMode(ApplicationMode.DEBUG);
        settings.setDeveloperMenuEnabled(true);
    }

    private Entity player1, player2, player3, player4;
    private PlayerComponent player1Comp, player2Comp, player3Comp, player4Comp;

    public int gameTime = 30;
    public int maxGameTime = 0;

    public static final int TILE_SIZE = 30;

    public List<Integer> codedMessage;

    public TimerAction maxGameTimer;

    static Crypto crypto = new Crypto();

    // Last 4 inputs will be logged
    public HashMap<String, Integer> firstPlayerKeyMap = new HashMap<String, Integer>() {{
        put("W", 0);
        put("A", 0);
        put("S", 0);
        put("D", 0);
        put("F", 0);
    }};

    public HashMap<String, Integer> secondPlayerKeyMap = new HashMap<String, Integer>() {{
        put("W", 0);
        put("A", 0);
        put("S", 0);
        put("D", 0);
        put("F", 0);
    }};

    public HashMap<String, Integer> thirdPlayerKeyMap = new HashMap<String, Integer>() {{
        put("W", 0);
        put("A", 0);
        put("S", 0);
        put("D", 0);
        put("F", 0);
    }};

    public HashMap<String, Integer> fourthPlayerKeyMap = new HashMap<String, Integer>() {{
        put("W", 0);
        put("A", 0);
        put("S", 0);
        put("D", 0);
        put("F", 0);
    }};

    HashMap<String, Integer> currentKeyMap = new HashMap<>();

    private Server<String> server;

    Server<String> getServer() { return server; };



    @Override
    protected void initGameVars(Map<String, Object> vars) {
        vars.put("player1lives", 3);
        vars.put("player2lives", 3);
        vars.put("player3lives", 3);
        vars.put("player4lives", 3);
        vars.put("numOfConnections", 0);
    }

    @Override
    protected void initGame() {
        Writers.INSTANCE.addTCPWriter(String.class, outputStream -> new MessageWriterS(outputStream));
        Readers.INSTANCE.addTCPReader(String.class, in -> new MessageReaderS(in));

        server = getNetService().newTCPServer(55555, new ServerConfig<>(String.class));

        crypto.generateKey();

        server.setOnConnected(connection -> {
            connection.addMessageHandlerFX(this);
            if (connection.getConnectionNum() == 1) {
                connection.send(crypto.encryptDecrypt("START," + crypto.getSecretKey()));
                connection.send("SETUP,1," + player1Comp.getEntity().getHeight());
            } else if (connection.getConnectionNum() == 2) {
                connection.send(crypto.encryptDecrypt("START," + crypto.getSecretKey()));
                connection.send("SETUP,2," + player2Comp.getEntity().getHeight());
            } else if (connection.getConnectionNum() == 3) {
                connection.send(crypto.encryptDecrypt("START," + crypto.getSecretKey()));
                connection.send("SETUP,3," + player3Comp.getEntity().getHeight());
            } else if (connection.getConnectionNum() == 4) {
                connection.send(crypto.encryptDecrypt("START," + crypto.getSecretKey()));
                connection.send("SETUP,4," + player4Comp.getEntity().getHeight());
            }
            // If new client is trying to connect but the slots are full send the message
            else
            {
                connection.send(crypto.encryptDecrypt("SERVER_IS_FULL"));
            }
            inc("numOfConnections", 1);

        });

        

        getGameWorld().addEntityFactory(new BombermanFactory());
        getGameWorld().addEntityFactory(new LevelFactory());
        Level level = getAssetLoader().loadLevel("level_01.txt", new TextLevelLoader(30, 30,'0'));
        getGameWorld().setLevel(level);
        getGameScene().setBackgroundColor(Color.rgb(0, 0, 5));

        initScreenBounds();
        initGameObjects();

        var t = new Thread(server.startTask()::run);
        t.setDaemon(true);
        t.start();
    }

    @Override
    protected void initPhysics() {
        getPhysicsWorld().setGravity(0, 0);

        CollisionHandler powerupPlayerHandler = new CollisionHandler(EntityType.POWERUP, EntityType.PLAYER) {
            @Override
            protected void onCollisionBegin(Entity powerup, Entity player) {

                server.broadcast("POWERUP_PICKED," + powerup.getX() + "," + powerup.getY());

                powerup.getComponent(PowerUpComponent.class).givePower(player);
                getGameWorld().removeEntity(powerup);
            }
        };


        getPhysicsWorld().addCollisionHandler(powerupPlayerHandler);

    }

    @Override
    protected void initUI() {
        MainUIController controller = new MainUIController();
        UI ui = getAssetLoader().loadUI("main.fxml", controller);

    }

    @Override
    protected void onUpdate(double tpf) {
        if (!server.getConnections().isEmpty()) {
            var message = "GAME_DATA," + player1.getY() + "," + player1.getX() + "," +
                    player2.getY() + "," + player2.getX() + "," + player3.getY() + "," + player3.getX()
                    + "," + player4.getY() + "," + player4.getX() + "," + player1Comp.getLives() + "," + player2Comp.getLives()
                    + "," + player3Comp.getLives() + "," + player4Comp.getLives();
                    server.broadcast(message);
        }
    }

    public void onBrickDestroyed(Entity brick)
    {
        int cellX = (int)brick.getX();
        int cellY = (int)brick.getY();
        server.broadcast("BRICK_DESTROYED," + cellX + "," + cellY);
        getGameWorld().removeEntity(brick);

        // 40% chance to spawn powerup
        if (FXGLMath.random(0,10) > 6) {
            spawn("powerup", cellX, cellY );
            server.broadcast("SPAWN_POWERUP," + cellX + "," + cellY);
        }


    }

    public void onPlayerDamaged(Entity player)
    {
        player.getComponent(PlayerComponent.class).lives--;
        server.broadcast("PLAYER_HIT");
        if(player.getComponent(PlayerComponent.class).getLives() <= 0)
        {
            getGameWorld().removeEntity(player);
            server.broadcast("PLAYER_LOST," + player.getComponent(PlayerComponent.class).getPlayerNum());
        }
    }


    private void initScreenBounds() {
        Entity walls = entityBuilder()
                .type(EntityType.WALL)
                .collidable()
                .buildScreenBounds(150);

        getGameWorld().addEntity(walls);
    }

    private void initGameObjects() {

        player1 = spawn("player", new SpawnData(65, 95));
        player2 = spawn("player", new SpawnData(725, 95));
        player3 = spawn("player", new SpawnData(65, 505));
        player4 = spawn("player", new SpawnData(725, 505));

        player1Comp = player1.getComponent(PlayerComponent.class);
        player1Comp.playerNum = 1;
        player2Comp = player2.getComponent(PlayerComponent.class);
        player2Comp.playerNum = 2;
        player3Comp = player3.getComponent(PlayerComponent.class);
        player3Comp.playerNum = 3;
        player4Comp = player4.getComponent(PlayerComponent.class);
        player4Comp.playerNum = 4;
    }

    @Override
    public void onReceive(Connection<String> connection, String message) {
        var tokens = message.split(",");
        Arrays.stream(tokens).skip(1).forEach(key -> {
            PlayerComponent playerEntity = null;
            var stopPlayer = key.substring(0, 1).equals(("W")) || key.substring(0,1).equals("S")
                    || key.substring(0,1).equals("A") || key.substring(0,1).equals("D");

            if (connection.getConnectionNum() == 1) {
                playerEntity = player1Comp;
                currentKeyMap = firstPlayerKeyMap;
                System.out.println("CONNECTION 1 DETECTED");
            }
            else if (connection.getConnectionNum() == 2) {
                playerEntity = player2Comp;
                currentKeyMap = secondPlayerKeyMap;
                System.out.println("CONNECTION 2 DETECTED");
            }
            else if (connection.getConnectionNum() == 3) {
                playerEntity = player3Comp;
                currentKeyMap = thirdPlayerKeyMap;
                System.out.println("CONNECTION 3 DETECTED");
            }
            else if (connection.getConnectionNum() == 4) {
                playerEntity = player4Comp;
                currentKeyMap = fourthPlayerKeyMap;
                System.out.println("CONNECTION 4 DETECTED");
            }
            else {
                playerEntity = player1Comp;
            }

            if (key.endsWith("_DOWN")) {
                if (key.substring(0,1).equals("W")) {
                    currentKeyMap.put("W", 1);
                    playerEntity.up();
                    System.out.println("COMMAND TO MOVE UP");
                }
                if (key.substring(0,1).equals("S")) {
                    currentKeyMap.put("S", 1);
                    playerEntity.down();
                    System.out.println("COMMAND TO MOVE DOWN");
                }
                if (key.substring(0,1).equals("A")) {
                    currentKeyMap.put("A", 1);
                    playerEntity.left();
                    System.out.println("COMMAND TO MOVE LEFT");
                }
                if (key.substring(0,1).equals("D")) {
                    currentKeyMap.put("D", 1);
                    playerEntity.right();
                    System.out.println("COMMAND TO MOVE RIGHT");
                }
                if (key.substring(0,1).equals("F")) {
                    currentKeyMap.put("F",1);
                    playerEntity.placeBomb();
                    System.out.println("COMMAND TO PLACE BOMB");
                }
            }
            if (key.endsWith("_UP")) {
                currentKeyMap.put(key.substring(0,1), 0);
                if(stopPlayer && !areKeysUp(currentKeyMap)){
                    playerEntity.stop();
                    System.out.println("COMMAND TO STOP");
                }
            }

            if (connection.getConnectionNum() == 1) {
                updateKeyMap(firstPlayerKeyMap, currentKeyMap);
                System.out.println("UPDATED 1 PLAYER KEYMAP");
            }
            else if (connection.getConnectionNum() == 2) {
                updateKeyMap(secondPlayerKeyMap, currentKeyMap);
                System.out.println("UPDATED 2 PLAYER KEYMAP");
            }
            else if (connection.getConnectionNum() == 3) {
                updateKeyMap(thirdPlayerKeyMap, currentKeyMap);
                System.out.println("UPDATED 3 PLAYER KEYMAP");
            }
            else if (connection.getConnectionNum() == 4) {
                updateKeyMap(fourthPlayerKeyMap, currentKeyMap);
                System.out.println("UPDATED 4 PLAYER KEYMAP");
            }
            else {playerEntity = player1Comp;}

            if (key.equals("CLOSED"))
            {
                connection.terminate();
                System.out.println("Client Closed");
            }

        });
    }


    public boolean areKeysUp (HashMap<String, Integer> currentKeyMap)
    {
        var pressedKeyNum = 0;
        for (Integer i : currentKeyMap.values()){
            if(i > 0)
            {
                pressedKeyNum++;
            }

        }
        if(pressedKeyNum > 0)
        {
            return true;
        }
        else {return false;}
    }

    public void updateKeyMap (HashMap<String, Integer> oldMap, HashMap<String,Integer> newMap)
    {
        for (String newKey : newMap.keySet())
        {
            for (String oldKey : oldMap.keySet())
            {
                if(newKey.equals(oldKey))
                {
                    oldMap.put(oldKey, newMap.get(oldKey));
                }
            }
        }
    }

    static class MessageWriterS implements TCPMessageWriter<String> {

        private OutputStream os;
        private PrintWriter out;

        MessageWriterS(OutputStream os) {
            this.os = os;
            out = new PrintWriter(os, true);
        }

        @Override
        public void write(String s) throws Exception {
            s = crypto.encryptDecrypt(s);
            out.print(s.toCharArray());
            out.flush();
        }
    }

    static class MessageReaderS implements TCPMessageReader<String> {

        private BlockingQueue<String> messages = new ArrayBlockingQueue<>(50);

        private InputStreamReader in;

        MessageReaderS(InputStream is) {
            in =  new InputStreamReader(is);

            var t = new Thread(() -> {
                try {

                    char[] buf = new char[36];

                    int len;

                    while ((len = in.read(buf)) > 0) {
                        var message = new String(Arrays.copyOf(buf, len));
                        message = crypto.encryptDecrypt(message);

                        System.out.println("Recv message: " + message);

                        messages.put(message);
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                }
            });

            t.setDaemon(true);
            t.start();
        }

        @Override
        public String read() throws Exception {
            return messages.take();
        }
    }

    public static void main(String[] args) {
        launch(args);
    }
}
