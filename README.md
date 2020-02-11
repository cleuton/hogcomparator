Comparação de rostos com Java e C++ utilizando HOG

[![](./banner_livros2.png)](https://www.lcm.com.br/site/#livros/busca?term=cleuton)

HOG - Histogram of Oriented Gradients (histograma de gradientes orientados) é um descritor de imagens, capaz de resumir as principais características de uma imagem, como rostos por exemplo, permitindo comparação com imagens semelhantes.

Este artigo e tutorial é de dois anos atrás e eu resolvi atualizar e modernizar o código-fonte para publicar novamente.

Um detalhe importante é que eu precisarei carregar os dois arquivos de modelo, que podem ser obtidos de:
http://dlib.net/files/shape_predictor_5_face_landmarks.dat.bz2
http://dlib.net/files/dlib_face_recognition_resnet_model_v1.dat.bz2

Basta descompactar e depois criar uma variável de ambiente chamada HOGCOMPARATOR_PATH, apontando para o path onde você descompactou os dois arquivos.

O site abaixo ensina a instalar e compilar a OpenCV:
https://docs.opencv.org/3.4.2/d7/d9f/tutorial_linux_install.html

Uma vez que a OpenCV esteja instalada, você pode compilar a parte C++. Para isto, copie o arquivo header gerado para dentro da pasta cplusplus (se você o alterou), e abra um terminal:
    1. cd hog/cplusplus
    2. mkdir build
    3. cd build
    4. cmake ..
    5. cmake --build . --config Release

Quando terminar de compilar, você terá um arquivo “libhogcomparator.so” dentro da pasta build. Esta é a biblioteca que implementa o método nativo.

Para executar o projeto no eclipse, abra o menu RUN e depois RUN CONFIGURARIONS. Crie uma configuração para executar “Java Application”, selecione a classe principal (HogComparator) e adicione dois argumentos, que são os paths das imagens que deseja comparar. Acrescente também um argumento para a JVM, que é o -Djava.library.path, apontando para a pasta cplusplus/build. Finalmente, crie a variável de ambiente apontando para o path onde você descompactou os dois arquivos de modelos. 
    • Argumentos de linha de comando, por exemplo: /home/cleuton/Documentos/projetos/hog/etc/cleuton.jpg /home/cleuton/Documentos/projetos/hog/etc/thomas_edison.jpg
    • Argumento de localização da “libhogcomparator”: -Djava.library.path=/home/cleuton/Documentos/projetos/hog/cplusplus/build
    • Variável de ambiente: HOGCOMPARATOR_PATH=/home/cleuton/Documentos/projetos/hog/cplusplus/build
