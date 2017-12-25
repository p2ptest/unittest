pipeline {
  agent any
  stages {
    stage('build') {
      parallel {
        stage('build') {
          steps {
            echo 'hah'
          }
        }
        stage('build-2') {
          steps {
            echo 'hiahia'
            echo 'heihei'
          }
        }
      }
    }
    stage('echo') {
      steps {
        sh 'echo "echo"'
      }
    }
  }
}