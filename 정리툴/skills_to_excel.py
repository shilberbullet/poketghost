import pandas as pd
from openpyxl import Workbook
from openpyxl.styles import PatternFill
from openpyxl.worksheet.table import Table, TableStyleInfo
from openpyxl.utils.dataframe import dataframe_to_rows

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

# 타입별 색상 지정
type_colors = {
    "EVIL_SPIRIT": "FF9999",  # 빨간색
    "GHOST": "CC99FF",        # 보라색
    "MONSTER": "FFFF99",      # 노란색
    "HUMAN": "66CCCC",        # 청록색
    "ANIMAL": "99CC99",       # 초록색
}

# 기술 데이터 파싱
skill_records = []
for line in skill_data.strip().splitlines():
    parts = line.split(",")
    if len(parts) >= 6:
        name = parts[0].strip()
        type_ = parts[1].strip()
        power = parts[2].strip()
        accuracy = parts[3].strip()
        pp = parts[4].strip()
        description = parts[5].strip()
        skill_records.append([name, type_, power, accuracy, pp, description])

# DataFrame 생성
df = pd.DataFrame(skill_records, columns=["이름", "타입", "위력", "명중률", "PP", "설명"])

# 워크북 생성
wb = Workbook()
ws = wb.active
ws.title = "기술 도감"

# 데이터 입력
for r in dataframe_to_rows(df, index=False, header=True):
    ws.append(r)

# 타입별 색상 적용
for row in ws.iter_rows(min_row=2, max_row=ws.max_row, min_col=2, max_col=2):
    for cell in row:
        color = type_colors.get(cell.value, None)
        if color:
            cell.fill = PatternFill(start_color=color, end_color=color, fill_type="solid")

# 표 추가
table = Table(displayName="SkillDex", ref=f"A1:F{ws.max_row}")
table.tableStyleInfo = TableStyleInfo(name="TableStyleMedium9", showRowStripes=True)
ws.add_table(table)

# 열 너비 자동 조정
for column in ws.columns:
    max_length = 0
    column = [cell for cell in column]
    for cell in column:
        try:
            if len(str(cell.value)) > max_length:
                max_length = len(str(cell.value))
        except:
            pass
    adjusted_width = (max_length + 2)
    ws.column_dimensions[column[0].column_letter].width = adjusted_width

# 저장
path = "기술_도감.xlsx"
wb.save(path)

print(f"기술 도감이 {path}에 저장되었습니다.") 