"""Rebuild and verify the submission without changing recorded evidence."""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--compiler',default=shutil.which('g++'))
    parser.add_argument('--task',type=int,choices=range(1,13))
    args=parser.parse_args()
    compiler=args.compiler
    fallback=Path(r'C:\Program Files (x86)\Dev-Cpp\MinGW64\bin\g++.exe')
    if not compiler and fallback.exists(): compiler=str(fallback)
    if not compiler: parser.error('g++ was not found; pass --compiler PATH')
    repo=Path(__file__).resolve().parents[1]
    build=repo/'build'
    build.mkdir(exist_ok=True)
    env=os.environ.copy()
    compiler_path=Path(compiler).resolve()
    env['PATH']=str(compiler_path.parent)+os.pathsep+env.get('PATH','')
    flags=['-std=c++14','-Wall','-Wextra','-pedantic','-O2']
    records=[]
    for task in ([args.task] if args.task else range(1,13)):
        folder=repo/f'Task{task:02}'
        specs=json.loads((folder/'runs.json').read_text())
        for spec in specs:
            source=folder/spec['source']
            binary=build/(f'Task{task:02}_{source.stem}'+('.exe' if os.name=='nt' else ''))
            cmd=[compiler,*flags,str(source),'-o',str(binary)]
            if spec.get('compile_failure'): cmd.insert(1,'-pedantic-errors')
            result=subprocess.run(cmd,cwd=repo,env=env,capture_output=True,text=True)
            if spec.get('compile_failure'):
                if result.returncode==0 or 'narrow' not in result.stderr.lower():
                    raise RuntimeError('Expected narrowing rejection missing')
                records.append({'task':task,'source':spec['source'],'expected_compile_rejection':True})
                print(f'Task{task:02}/{spec["source"]}: expected compile rejection PASS')
                continue
            if result.returncode: raise RuntimeError(result.stderr)
            if spec['source']!='predict.cpp' and result.stderr.strip():
                raise RuntimeError('Unexpected diagnostics: '+result.stderr)
            if spec.get('prediction'):
                expected=(folder/spec['prediction']).read_text()
            for case in spec.get('cases',[{'name':'run','input':'input.txt'}]):
                executed=subprocess.run([str(binary),*case.get('args',[])],cwd=folder,env=env,
                    input=(folder/case['input']).read_text(),capture_output=True,text=True,timeout=60)
                if executed.returncode!=case.get('exit',0): raise RuntimeError('Unexpected exit: '+spec['source'])
                if 'expected' in case and executed.stdout!=case['expected']: raise RuntimeError('Output mismatch: '+case['name'])
                if spec.get('prediction') and executed.stdout!=expected: raise RuntimeError('Prediction mismatch')
                for fragment in case.get('expected_contains',[]):
                    if fragment not in executed.stdout: raise RuntimeError('Missing output: '+fragment)
                records.append({'task':task,'source':spec['source'],'case':case['name'],'exit':executed.returncode,'passed':True})
                print(f'Task{task:02}/{spec["source"]}/{case["name"]}: PASS')
    (build/'verification.json').write_text(json.dumps(records,indent=2))
    print(f'PASS: {len(records)} execution/diagnostic cases; original evidence files unchanged.')

if __name__=='__main__':
    try: main()
    except Exception as error:
        print('FAIL:',error,file=sys.stderr)
        sys.exit(1)
