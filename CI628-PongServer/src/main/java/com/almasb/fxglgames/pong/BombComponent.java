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
import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.physics.PhysicsComponent;
import com.almasb.fxgl.entity.components.BoundingBoxComponent;
import com.almasb.fxglgames.pong.BombermanApp;

import static com.almasb.fxgl.dsl.FXGL.*;
import static java.lang.Math.abs;
import static java.lang.Math.signum;

/**
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public class BombComponent extends Component {

    private int radius;
    private PhysicsComponent physics;

    public BombComponent(int radius)
    {
        this.radius = radius;
    }



    public void explode()
    {

        BoundingBoxComponent bbox = entity.getBoundingBoxComponent();
        // Destroy bricks
        getGameWorld()
                .getEntitiesInRange(bbox.range(radius, radius))
                .stream()
                .filter(e -> e.isType(EntityType.BRICK))
                .forEach(e -> {
                    if( Math.abs( e.getX() - bbox.getEntity().getX()) < 20 || Math.abs(e.getY() - bbox.getEntity().getY()) < 20)
                    {
                        FXGL.<BombermanApp>getAppCast().onBrickDestroyed(e);
                        e.removeFromWorld();
                    }
                });

        // Damage players
        getGameWorld()
                .getEntitiesInRange(bbox.range(radius, radius))
                .stream()
                .filter(e -> e.isType(EntityType.PLAYER))
                .forEach(e -> {
                    if( Math.abs( e.getX() - bbox.getEntity().getX()) < 20 || Math.abs(e.getY() - bbox.getEntity().getY()) < 20)
                    {
                        FXGL.<BombermanApp>getAppCast().onPlayerDamaged(e);
                    }
                });
        FXGL.<BombermanApp>getAppCast().getServer().broadcast("BOMB_EXPLODED," + bbox.getEntity().getX() + "," + bbox.getEntity().getY() + "," + radius);
        System.out.println("PLAYER_BOMB_RADIUS NOW: " + radius);
        entity.removeFromWorld();
    }

}
