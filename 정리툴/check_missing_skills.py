import pandas as pd

def load_skill_data():
    # 기술 데이터
    skill_data = '''
불꽃치기,MONSTER,30,90,20,불꽃으로 적을 공격한다
돌던지기,HUMAN,25,95,25,돌을 던져 적을 공격한다
꼬리치기,ANIMAL,20,100,25,꼬리로 적을 공격한다
물기,ANIMAL,25,90,20,이빨로 적을 물어뜯는다
할퀴기,ANIMAL,25,90,20,날카로운 발톱으로 할퀴어 공격한다
몸통박치기,MONSTER,30,85,20,몸 전체로 부딪혀 공격한다
독침,ANIMAL,20,95,30,독침을 쏘아 공격한다
짖기,ANIMAL,15,100,30,큰 소리로 적의 명중률을 낮춘다
음파,GHOST,25,90,25,강한 음파로 정신 피해를 입힌다
유혹하기,ANIMAL,15,85,30,적을 유혹하여 공격력을 낮춘다
날개베기,ANIMAL,30,90,15,날개를 이용해 회전 베기를 가한다
울부짖기,ANIMAL,0,100,5,자신의 공격력을 2턴간 증가시킨다
연속박치기,MONSTER,25×2,90,6,두 번 연속으로 머리로 들이받는다
속임수베기,HUMAN,35,100,10,기습 공격으로 방어력을 무시한다
망령의눈빛,GHOST,0,85,4,적을 1턴간 행동불능으로 만들 확률 30%
바람칼,HUMAN,35,85,20,바람으로 만든 칼날을 날린다
얼음창,GHOST,40,80,15,얼음으로 만든 창을 던진다
물어뜯기,ANIMAL,35,80,20,이빨로 적을 물어뜯는다
후려치기,HUMAN,35,80,20,무거운 일격으로 적을 가격한다
맹독침,ANIMAL,35,80,20,강한 독을 가진 독침을 쏘아 공격한다
흙폭탄,HUMAN,30,90,20,흙으로 만든 폭탄을 던진다
마구 할퀴기,ANIMAL,40,85,10,연속으로 할퀴어 강한 피해를 준다
염력,GHOST,35,85,20,염력으로 물건을 던져져 공격한다
지옥불,EVIL_SPIRIT,50,70,5,지옥의 불꽃으로 적을 태운다
가시돌진,MONSTER,40,85,15,몸의 가시로 돌진하며 상대에게 큰 피해를 준다
분노의주먹,HUMAN,45,80,10,분노를 담아 강한 한 방을 날린다
사념의박치기,MONSTER,50,80,5,강하게 부딪힌다다
강철꼬리치기,MONSTER,45,80,10,단단한 꼬리로 강하게 내리친다
맹화,MONSTER,50,85,5,강한 불꽃으로 공격헌다
깨물어부수기,ANIMAL,40,90,15,이빨로 깨물어 부순다다
번개,GHOST,45,75,10,번개로 적을 공격한다
철퇴내리치기,HUMAN,50,75,5,무거운 철퇴를 휘둘러 큰 피해를 준다
지옥기운,EVIL_SPIRIT,60,65,5,지옥의 어둠으로 적에게 큰 피해를 준다
이단도약,MONSTER,45,85,10,두 번 튕겨 뛰며 큰 피해를 준다
'''
    
    # 기술 데이터 파싱
    skill_records = []
    for line in skill_data.strip().splitlines():
        parts = line.split(",")
        if len(parts) >= 2:
            skill_name = parts[0].strip()
            skill_type = parts[1].strip()
            skill_records.append([skill_name, skill_type])
    
    # 기술 DataFrame 생성
    df_skills = pd.DataFrame(skill_records, columns=["이름", "타입"])
    return set(df_skills["이름"].tolist())

def load_yokai_data():
    # 요괴 데이터
    extended_text = '''
태자귀,GHOST,120,90,120,110,원한을 품고 저승을 떠도는 무서운 원귀. 확률적으로 강력한 일격을 가하는 특성을 지녔다.,천연두;울음소리;복화술;염매
총각귀신,GHOST,130,110,110,120,죽지 못하고 떠도는 처절한 원귀, 강한 물리 공격과 방어력 저하 기술을 사용한다.,메치기;포효;위압;버티기
처녀귀신,GHOST,140,100,100,110,불행한 죽음으로 원한 가득한 처녀귀신. 할퀴기와 공격력 및 방어력 저하 스킬이 특징이다.,할퀴기;비명;공포;원한의 바람
구미호,ANIMAL,140,80,100,150,매혹적인 외모와 여우불로 적을 홀리는 구미호는 변신술과 흡생 능력을 지닌 전설의 요괴다.,매혹;할퀴기;간 빼먹기;둔갑술
망태 할아버지,HUMAN,100,110,130,120,밤마다 어린이를 납치해 망태에 넣는다는 무서운 전설의 주인공 인간형에게 특히 위협적이다.,납치;민첩한 걸음;그물 던지기;
돌진천마,ANIMAL,110,100,110,150,옥황상제가 타던 신성한 천마. 하늘을 달리며 전장을 누비고 아군에게 희망을 안겨주는 존재.,천상 질주;신광 충돌;포효하는 발굽;하늘의 응답
마귀,EVIL_SPIRIT,125,95,100,130,형체도 불분명한 사악한 기운의 집합체. 조용히 상대의 정신을 갉아먹는 공포의 존재.,혼령 착취;서늘한 시선;틈 속의 손;영혼 긁기
불가살이,MONSTER,135,135,145,75,강철 피부와 불사의 힘으로 죽지 않는 몸을 지녔다. 물리 공격을 받을수록 더 강해지는 무시무시한 괴수.,강철 섭취;무쇠 손톱;강철 피부;불사의 포효;강철 포식
'''
    
    # 요괴 데이터 파싱
    yokai_records = []
    for line in extended_text.strip().splitlines():
        parts = line.split(",")
        if len(parts) < 7:
            continue
        name = parts[0].strip()
        skills = [s.strip() for s in parts[7:][0].split(";") if s.strip()]
        yokai_records.append([name, skills])
    
    return yokai_records

def check_missing_skills():
    # 기술 데이터와 요괴 데이터 로드
    known_skills = load_skill_data()
    yokai_data = load_yokai_data()
    
    # 누락된 기술 확인
    missing_skills = {}
    for yokai_name, skills in yokai_data:
        missing = [skill for skill in skills if skill not in known_skills]
        if missing:
            missing_skills[yokai_name] = missing
    
    # 결과 출력
    if missing_skills:
        print("\n=== 누락된 기술 목록 ===")
        for yokai_name, skills in missing_skills.items():
            print(f"\n{yokai_name}:")
            for skill in skills:
                print(f"  - {skill}")
    else:
        print("\n모든 기술이 기술 데이터에 포함되어 있습니다.")

if __name__ == "__main__":
    check_missing_skills() 