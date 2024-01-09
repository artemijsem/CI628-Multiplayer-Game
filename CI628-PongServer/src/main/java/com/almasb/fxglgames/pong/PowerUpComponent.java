package com.almasb.fxglgames.pong;

import com.almasb.fxgl.entity.Entity;
import com.almasb.fxgl.entity.component.Component;
import com.almasb.fxgl.physics.PhysicsComponent;

/**
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public class PowerUpComponent extends Component {

    private PhysicsComponent physics;

    public void givePower(Entity player)
    {
        player.getComponent(PlayerComponent.class).bombRadius += 5;
    }

}
