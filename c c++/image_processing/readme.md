# Kleine Bibliothek für Bildverarbeitung

# Inhaltsverzeichnis
1. [Bild erzeugen](#Bilderzeugung)
2. 
Mit dieser Bibliothek kann zum Beispiel:

1. Bilder geladen und geschrieben.
2. Bildhistogramm in der Konsole ausgeben ([histogramm](data/screenshot.png)).
3. Sobel-([sobel](data/sobel.png)), Gauß-Filter ([gauß](data/gaussian_blur.png)), Boxfilter und allgemeine
   Filteroperation ausgeführt werden.
4. Bilder können auch addiert (**+**), subtrahiert (**-**), multipliziert (**x**)([multiply](data/corner.png))

[comment]: <> (![git]&#40;data/gif.gif&#41;)
![git](data/video.gif)

# Bilderzeugung <a name="Bilderzeugung"></a>

```c
Image(int n_rows, int n_cols, int n_channels);
```

- *n_rows*: Die Anzahl von Zeilen.
- *n_cols*: Die Anzahl von Spalten.
- *n_channels*: Die Anzahl von Kanälen.

```c
Image(const char *filename);
```

- *filename*: Das Path zur Bilddatei.

```c
Image(const Image &image);
```

- *image*: Das Bild, das den Inhalt kopiert werden soll.

## Hilfsfunktion

- Die Anzahl von Zeilen.
    ```c 
        int rows_() const;
    ```
    ```c  
        int height() const;
    ```
- Die Anzahl von Spalten
    ```c
        int cols_() const;
    ```
    ```c
        int width() const;
    ```
- Die Anzahl von Pixels im Bild.
    ```c
        int size_() const;
    ```
- Die Anzahl von Kanälen.
    ```c
        int getChannels() const;
    ```

## Graustufen

| Befehl | Beispiel |
|:---: |---|
| `image.gray()` <br>  `image.toGray()`  <br> `image.brightness()`  | ![gray](data/gray.png) |

## Weitere Funktionen

| Befehl | Beispiel | Befehl | Beispiel |
| :---: | :---: | :---: | :---: |
| image.blur() | ![blur](data/blur.png) | image.invert() | ![invert](data/invert.png)
