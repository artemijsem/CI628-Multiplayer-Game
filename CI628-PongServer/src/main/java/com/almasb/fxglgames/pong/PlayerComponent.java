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

import com.almasb.fxgl.dsl.FXGL;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxglgames.pong.BombComponent;
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.SpawnData;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxglgames.pong.BombermanApp;
import javafx.util.Duration;

import static com.almasb.fxgl.dsl.FXGL.getGameTimer;
import static com.almasb.fxgl.dsl.FXGL.spawn;
/**
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public class PlayerComponent extends Component {

    private static final double BAT_SPEED = 60;

    protected PhysicsComponent physics;

    public int playerNum = 0;

    public int lives = 3;

    public int maxBombs = 1;

    public int bombsPlaced = 0;
    public int bombRadius = 30;

    public void up() {
 /*       if (entity.getY() >= BAT_SPEED / 60)*/
            physics.setVelocityY(-BAT_SPEED);
    }

    public void down() {
        /*if (entity.getBottomY() <= FXGL.getAppHeight() - (BAT_SPEED / 60))*/
            physics.setVelocityY(BAT_SPEED);
    }

    public void left() {
        /*if (entity.getX() >= BAT_SPEED / 60)*/
            physics.setVelocityX(-BAT_SPEED);
    }

    public void right() {
        /*if (entity.getX() >= BAT_SPEED / 60)*/
            physics.setVelocityX(BAT_SPEED);
    }

    public void stop() {

        physics.setLinearVelocity(0,0);

    }
    public void placeBomb() {
        if (bombsPlaced == maxBombs) {
            return;
        }

        bombsPlaced++;

        Entity bomb = spawn("bomb", new SpawnData(this.getEntity().getX(), this.getEntity().getY()).put("radius", bombRadius));
        FXGL.<BombermanApp>getAppCast().getServer().broadcast("SPAWN_BOMB," + this.getEntity().getX() + "," + this.getEntity().getY());

        getGameTimer().runOnceAfter(() -> {
            bomb.getComponent(BombComponent.class).explode();
            bombsPlaced--;
        }, Duration.seconds(2));
    }

    public int getLives() {return lives;}

    public int getPlayerNum() {return playerNum;}
}
