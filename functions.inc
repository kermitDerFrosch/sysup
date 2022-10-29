function logLine() {
  pane=$1
  shift
  if [ ${#TMUX} -gt 0 ];then
    [ $pane -eq 2 ] && [ $FIRSTLOG -eq 1 ] && FIRSTLOG=0 && echo -e "\n\n" >> /tmp/sysup.pane_$pane
    printf "\n${@/\\n/}" >> /tmp/sysup.pane_$pane
  else
    printf "$@" >&2
  fi
  printf "$@" >> "${LOGFILE}"
  unset pane
}

function usage() {
  echo -e "USAGE: $0 [-nc] [-ns] [-np] [-nr] [-fp] [-r] [-q] [-na]\n"
  echo "Options:"
  echo "    --nosync, -ns      - skip sync phase"
  echo "    --noportage, -np   - skip portage update phase"
  echo "    --norebuild, -nr   - skip rebuild phase"
  echo "    --noclean, -nc     - skip clean phase"
  echo "    --noask, -na       - don't ask"
  echo "    --quiet, -q        - hide output"
  echo
  echo "    --fixperl, -fp     - fix perl"
  echo "    --resume, -r       - resume"
  echo "    --skipfirst, -sf   - skip first"
  echo
  echo "    --sync, -s         - sync phase"
  echo "    --portage, -p      - portage update phase"
  echo "    --rebuild, -rb     - rebuild phase"
  echo "    --clean, -c        - clean phase"
  exit 0
}

# show duration per part
function printDuration() {
  local totaltime=0
  local MAX=0
  for TIME in ${TIMES[@]};do
    let totaltime=totaltime+TIME
  done
  for NAME in ${PARTNAMES[@]};do
    if [ $MAX -lt ${#NAME} ];then
      MAX=${#NAME}
    fi
  done

  echo
  local -A LEN
  for PART in ${PARTS[@]};do
    if [[ ${TIMES[$PART]} > 0 ]];then
      local len
      local offset
      local offset2
      LEN[$PART]=${#PARTNAMES[$PART]}

      if [ ${LEN[$PART]} -gt 8 ]; then
        let len=${LEN[$PART]}+2
      else
        len=10
      fi

      let offset=($len-${LEN[$PART]})/2
      if [ $((${LEN[$PART]}%2)) -eq 0 ];then
        offset2=$offset
      else
        let offset2=offset+1
      fi
      printf "\e[4m%${offset}s%s%${offset2}s" " " "${PARTNAMES[$PART]}" " " >&2
      if [[ "$PART" != "${PARTS[-1]}" ]];then
        printf "|" >&2
      fi
    fi
  done
  printf "\e[0m\n" >&2
  for PART in ${PARTS[@]};do
    if [[ ${TIMES[$PART]} > 0 ]];then
      local offset=1
      local offset2
      if [ ${LEN[$PART]} -gt 8 ];then
        let offset=(${LEN[$PART]}-6)/2
      fi
      if [[ $((${LEN[$PART]}%2)) -eq 0 ]];then
        offset2=$offset
      else
        let offset2=offset+1
      fi

      printf "%${offset}s" " " >&2
      DD=`date -u -d @${TIMES[$PART]} "+%H:%M:%S"`
      echo -ne "\e[1m$DD\e[0m" >&2
      printf "%${offset2}s" " " >&2
      if [[ "$PART" != "${PARTS[-1]}" ]];then
        echo -n "|" >&2
      fi
    fi
  done

  echo -ne "\n\nTotal: \e[1m" >&2
  date -u -d @$totaltime "+%-Hh %-Mm %-Ss" >&2
  echo -en "\e[0m"
}

# Check return codes
function checkError() {
  if [ $1 != 0 ];then
    if [[ "x$2" != "x" ]];then
      echo -e "[\e[1;31mE\e[0m] $2 ($1)" >&2
      [ ${USE_TMUX} -gt 0 ] && echo -e "[\e[1;31mE\e[0m] $2 ($1)" >> /tmp/sysup.log
    fi
    exit $1
  fi
}

# Start a subprocess and pipe output to a log
function startSubProcess() {
  if [ $CLEANED -eq 1 ] || [ $START -ne 1 ]; then
    return
  fi
  if [ ${CMDS[$3$1]+x} ];then
    if [ $QUIET -eq 1 ];then
      ${CMDS[$3$1]} 2>&1 >> "$2"
      RC[$3$1]=${PIPESTATUS[0]}
    else
      ${CMDS[$3$1]} 2>&1 | tee -ia "$2"
      RC[$3$1]=${PIPESTATUS[0]}
    fi
  fi
}

# Run a single part with pre- and post- script if defined
function processPart() {
  if [ $CLEANED -eq 1 ] || [ $START -ne 1 ]; then
    return
  fi

  local endtime
  local starttime=`date  "+%s"`
  local LOGFILE="$LOGPATH/${1,,}_$(eval date +%Y%m%d%H%M%S).log"
  
  logLine 2 "\e[1m[%s - %s] Starting %s Phase\e[0m\n" "$(eval date +%d.%m.%Y)" "$(eval date +%R:%S)" "${PARTNAMES[$1]}"

  startSubProcess "$1" "$LOGFILE" "PRE"
  startSubProcess "$1" "$LOGFILE"
  startSubProcess "$1" "$LOGFILE" "POST"

  endtime=`date  "+%s"`
  let TIMES[$1]=endtime-starttime
  logLine 2 "[%s - %s] %s Phase Done\n" "$(eval date +%d.%m.%Y)" "$(eval date +%R:%S)" "${PARTNAMES[$1]}"
  checkError ${RC[$1]} ${ERROR_MESSAGE[$1]}
}

# parse paramter
function parseParameter() {
  var="${1^^}"
  if [ "x${1:0:2}" != "x--" ];then
    if [ "x${1:0:1}" != "x-" ];then
      echo "unknown argument: $1"
      exit 1
    fi
    var="${var:1}"
  else
    var="${var:2}"
  fi
  case "${var}" in
    NOSYNC|NS)
      NOSYNC=1
      ;;
    SYNC|S)
      SYNC=1
      ;;
    NOASK|NA)
      NOASK=1
      ;;
    QUIET|Q)
      QUIET=1
      ;;
    NOPORTAGE|NP)
      NOPORTAGE=1
      ;;
    PORTAGE|P)
      PORTAGE=1
      ;;
    UPDATE|U)
      UPDATE=1
      ;;
    NOREBUILD|NR)
      NOREBUILD=1
      ;;
    REBUILD|RB)
      REBUILD=1
      ;;
    NOCLEAN|NC)
      NOCLEAN=1
      ;;
    CLEAN|C)
      CLEAN=1
      ;;
    SKIPFIRST|SF)
      SKIPFIRST=1
      ;;
    HELP|H)
      usage
      ;;
    FIXPERL|FP)
      NOSYNC=1
      NOPORTAGE=1
      NOREBUILD=1
      NOCLEAN=1
      FIXPERL=1
      NOUPDATE=1
      break;
      ;;
    RESUME|R)
      NOSYNC=1
      NOPORTAGE=1
      NOREBUILD=0
      NOCLEAN=0
      NOUPDATE=1
      RESUME=1
      break;
      ;;
    *)
      echo "wrong parameter: $var"
      exit 2
      ;;
  esac
}

# check parameter validity
function checkParameters() {
  if [ $(($REBUILD+$UPDATE+$CLEAN+$SYNC+$PORTAGE)) -gt 0 ] && [ $((RESUME+FIXPERL)) -gt 0 ];then
    echo -e "\e[33mdon't know what to do :/\e[0m" >&2
    exit 1  
  fi

  # disable not needed parts
  if [ $(($REBUILD+$UPDATE+$CLEAN+$SYNC+$PORTAGE)) -gt 0 ];then
    NOSYNC=1
    NOPORTAGE=1
    NOUPDATE=1
    NOREBUILD=1
    NOCLEAN=1
  fi
}

# configure parts
function configureParts() {
  if [ $NOSYNC -eq 0 ] || [ $SYNC -eq 1 ];then
    PARTS[${#PARTS[@]}]="SYNC"
  fi
  if [ $NOPORTAGE -eq 0 ] || [ $PORTAGE -eq 1 ];then
    PARTS[${#PARTS[@]}]="PORTAGE"
  fi
  if [ $NOUPDATE -eq 0 ] || [ $UPDATE -eq 1 ];then
    PARTS[${#PARTS[@]}]="UPDATE"
  fi
  if [ $RESUME -eq 1 ];then
    PARTS[${#PARTS[@]}]="RESUME"
  fi
  if [ $NOREBUILD -eq 0 ] || [ $REBUILD -eq 1 ];then
    PARTS[${#PARTS[@]}]="REBUILD"
  fi
  if [ $NOCLEAN -eq 0 ] || [ $CLEAN -eq 1 ];then
    PARTS[${#PARTS[@]}]="CLEAN"
  fi
  if [ $FIXPERL -eq 1 ];then
    PARTS[${#PARTS[@]}]="FIXPERL"
  fi
  if [ $QUIET -eq 0 -a $NOASK -eq 0 ];then
    ASK=" -a"
  fi
  if [ $SKIPFIRST -eq 1 ];then
    RESUME_PARAMS="--skipfirst"
  fi
}

# load tmux panes
function startTmux() {
  TMUXBIN=$(which tmux 2>/dev/null)
  if [ ${#TMUX} -eq 0 ] && [ ${#TMUXBIN} -gt 0 ];then
    USE_TMUX=1
    exec $TMUXBIN new-session -s sysup '[ -f /tmp/sysup.pane_1 ] || > /tmp/sysup.pane_1;tail -n0 -f /tmp/sysup.pane_1' \; \
     split-window '[ -f /tmp/sysup.pane_2 ] || > /tmp/sysup.pane_2;tail -n0 -f /tmp/sysup.pane_2' \; \
     split-window "\"$(realpath "$0")\" $PARAMS; rm /tmp/sysup.pane_*; read -n1 -p'Warte auf Tastendruck...' -s;tmux kill-session" \; \
     resize-pane -t0 -y1 \; \
     resize-pane -t1 -y4
  fi
}

# get root if needed
function getRoot() {
  if [ $EUID -ne 0 ];then
    SUDO_BIN=$(which sudo 2>/dev/null)
    [ -z $SUDO_BIN ] && exec $(which su) - -c "\"$(realpath "$0")\" $PARAMS" || exec ${SUDO_BIN} "$(realpath "$0")" $PARAMS
    echo "undefined error" >&2
    exit 1
  fi
}
