package com.almasb.fxglgames.pong;

/**
 * @author Artemi Sementsenko (artemijsem) (artsem0107@gmail.com)
 */
public interface CryptoInterface {


    String encryptDecrypt(String data_string);

    void generateKey();

    int getSecretKey();
}
