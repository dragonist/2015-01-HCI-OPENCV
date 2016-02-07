# 2015-01-HCI-OPENCV
Shaking Your Body

##if You just want to see    
go Here!    
https://www.youtube.com/watch?v=8umxVte4Jeg    

##if You want to do it    
first, generate openframeworks with openCV   


second, drop this file into src


thirt, go here


https://github.com/dragonist/2015-01-HCI-OPENCV_P 


and drop bin into your project


run!

# 제작동기
```
우리는 가만히 노래를 듣고 있습니다. 신나는 노래조차 가만히 듣고 있 습니다. 흥겨운 노래를 가만히 듣는다는건 노래에 대한 예의가 아니라 고 생각했습니다. 그래서 움직여야만 들을수 있는 쥬크박스를 만들어 보았습니다.
내가 움직이는 정도에 따라 사운드 크기가 조절되고 노래를 온전히 내 것으로 만드는 날까지 노래는 계속 됩니다. 온전히 내것이 되었다 판단 되면 노래는 알아서 바뀝니다.
```
## 기술조사
```
서경진 교수님 : 도움이 될만한 무언가 많은 조언들 지훈오빠 : c++ vector
신영언니 : 무진장 많은 음악 CD
하챙 : 음원 편집기
openframworks 예제들 :
1. Graphics (imageLoader, imageSaver, fonts)
2. Sound (soundPlayer, soundPlayerFFT)
3. Video (videoGrabber)
```

## 시스템 구성도
- 카메라, 음원 파일들, 몸뚱아리, 컴퓨터

## 동작 방식 및 순서
```

원본 이미지를 디텍킹해서 움직이는 것의 테두리만 흰색으로 한다. 흰태두리 이미지를 시간차를 두고 천천히 바뀌게 하는 스르륵 이미지 를 만든다.
스르륵 이미지의 희색 픽셀의 갯수에 따라 사운드소리가 커지고 일정 수치 이상이면 카메라가 찍히면서 이미지를 저장한다. 이미지는 벡터에 차근 차근 저장 되고 있고, 카메라가 찍히는 것 과 동시에 왼쪽에 원 의 속도를 올려준다. 움직이지 않으면 원은 위로 올라가지 않고, 원이 가장 꼭대기에 다으면 다음 노래가 재생 된다.
```
    
